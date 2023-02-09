#IMPORT LIBRARY
#bokeh serve --show json1.py
import requests
import json
import pandas as pd
from bokeh.plotting import figure, show
from bokeh.models import HoverTool,LabelSet,ColumnDataSource
from bokeh.tile_providers import get_provider, STAMEN_TERRAIN
import numpy as np
from bokeh.server.server import Server
from bokeh.application import Application
from bokeh.application.handlers.function import FunctionHandler

#FUNCTION TO CONVERT GCS WGS84 TO WEB MERCATOR
#DATAFRAME
def wgs84_to_web_mercator(df, lon="long", lat="lat"):
    k = 6378137
    df["x"] = df[lon] * (k * np.pi/180.0)
    df["y"] = np.log(np.tan((90 + df[lat]) * np.pi/360.0)) * k
    return df

#POINT
def wgs84_web_mercator_point(lon,lat):
    k = 6378137
    x= lon * (k * np.pi/180.0)
    y= np.log(np.tan((90 + lat) * np.pi/360.0)) * k
    return x,y

#AREA EXTENT COORDINATE WGS4
lon_min,lat_min=1.974,46.038
lon_max,lat_max=3.748,49.214

#COORDINATE CONVERSION
xy_min=wgs84_web_mercator_point(lon_min,lat_min)
xy_max=wgs84_web_mercator_point(lon_max,lat_max)

#COORDINATE RANGE IN WEB MERCATOR
x_rangea,y_rangea=([xy_min[0],xy_max[0]], [xy_min[1],xy_max[1]])


#REST API QUERY
user_name='mouma'
password='momo19800712'
url_data='https://'+user_name+':'+password+'@opensky-network.org/api/states/all?'+'lamin='+str(lat_min)+'&lomin='+str(lon_min)+'&lamax='+str(lat_max)+'&lomax='+str(lon_max)


flight_source = ColumnDataSource({
    'icao24':[],'callsign':[],'origin_country':[],
    'time_position':[],'last_contact':[],'long':[],'lat':[],
    'baro_altitude':[],'on_ground':[],'velocity':[],'true_track':[],
    'vertical_rate':[],'sensors':[],'geo_altitude':[],'squawk':[],'spi':[],'position_source':[],'x':[],'y':[],
    'rot_angle':[],'url':[]
    })

"""

# UPDATING FLIGHT DATA
def update():
    response=requests.get(url_data).json()
    #LOAD TO PANDAS DATAFRAME
    col_name=['icao24','callsign','origin_country','time_position','last_contact','long','lat','baro_altitude','on_ground','velocity',       
    'true_track','vertical_rate','sensors','geo_altitude','squawk','spi','position_source']
    flight_df=pd.DataFrame(response['states'])
    flight_df=flight_df.loc[:,0:16]
    flight_df.columns=col_name
    flight_df=flight_df.fillna('No Data') #replace NAN with No Data
    flight_df.head()
    # CONVERT TO BOKEH DATASOURCE AND STREAMING
    n_roll=len(flight_df.index)
    print(n_roll)
    #print(flight_df.to_dict(orient='list'))
    flight_source.stream(flight_df.to_dict(orient='list'),n_roll)

"""

#//////////////////////////////////////////
def update():
    response=requests.get(url_data).json()
    #LOAD TO PANDAS DATAFRAME
    col_name=['icao24','callsign','origin_country','time_position','last_contact','long','lat','baro_altitude','on_ground','velocity',       
    'true_track','vertical_rate','sensors','geo_altitude','squawk','spi','position_source']
    flight_df=pd.DataFrame(response['states'])
    flight_df=flight_df.loc[:,0:16]
    flight_df.columns=col_name
    wgs84_to_web_mercator(flight_df)       
    flight_df=flight_df.fillna('No Data')
    flight_df['rot_angle']=flight_df['true_track']*-1
    icon_url='https://www.logolynx.com/images/logolynx/4c/4c507d3961382eefd8a48f5c387a0ee3.png' #icon url
    flight_df['url']=icon_url

    #flight_df.to_csv('myFile.csv', sep = '\t')
    # CONVERT TO BOKEH DATASOURCE AND STREAMING
    n_roll=len(flight_df.index)
    print(n_roll)
    #print(flight_df.to_dict(orient='list'))
    flight_source.stream(flight_df.to_dict(orient='list'),n_roll)

#//////////////////////////////////////////



response=requests.get(url_data).json()
#LOAD TO PANDAS DATAFRAME
col_name=['icao24','callsign','origin_country','time_position','last_contact','long','lat','baro_altitude','on_ground','velocity',       
'true_track','vertical_rate','sensors','geo_altitude','squawk','spi','position_source']
flight_df=pd.DataFrame(response['states'])
flight_df=flight_df.loc[:,0:16]
flight_df.columns=col_name
wgs84_to_web_mercator(flight_df)       
flight_df=flight_df.fillna('No Data')
flight_df['rot_angle']=flight_df['true_track']*-1
#icon_url='https:...' #icon url
icon_url='https://www.logolynx.com/images/logolynx/4c/4c507d3961382eefd8a48f5c387a0ee3.png' #icon url
flight_df['url']=icon_url

flight_df.to_csv('myFile.csv', sep = '\t')
# CONVERT TO BOKEH DATASOURCE AND STREAMING
n_roll=len(flight_df.index)
print(n_roll)
#print(flight_df.to_dict(orient='list'))
flight_source.stream(flight_df.to_dict(orient='list'),n_roll)


#CALLBACK UPATE IN AN INTERVAL
from bokeh.io import curdoc
curdoc().add_periodic_callback(update, 5000) #5000 ms/10000 ms for registered user . 


#PLOT AIRCRAFT POSITION
#p=figure(x_range=x_rangea,y_range=y_rangea,x_axis_type='mercator',y_axis_type='mercator',sizing_mode='scale_width',plot_height=300)
p=figure(x_range=x_rangea,y_range=y_rangea)
tile_prov=get_provider(STAMEN_TERRAIN)
p.add_tile(tile_prov,level='image')
p.image_url(url='url', x='x', y='y',source=flight_source,anchor='center',angle_units='deg',angle='rot_angle',h_units='screen',w_units='screen',w=40,h=40)
p.circle('x','y',source=flight_source,fill_color='red',hover_color='yellow',size=10,fill_alpha=0.8,line_width=0)

#ADD HOVER TOOL AND LABEL
my_hover=HoverTool()
my_hover.tooltips=[('Call sign','@callsign'),('Origin Country','@origin_country'),('velocity(m/s)','@velocity'),('Altitude(m)','@baro_altitude')]
#labels = LabelSet(x='x', y='y', text='callsign', level='glyph',
#x_offset=5, y_offset=5, source=flight_source, render_mode='canvas',background_fill_color='white',text_font_size="8pt")
labels = LabelSet(x='x', y='y')
p.add_tools(my_hover)
p.add_layout(labels)



    
curdoc().title='REAL TIME FLIGHT TRACKING'
curdoc().add_root(p)




#show(p)
