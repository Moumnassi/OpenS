# Streaming Data (Generate Fake Data)
# Digital Ocean - Server App


from bokeh.io import curdoc
from bokeh.models import ColumnDataSource, DatetimeTickFormatter, Select
from bokeh.layouts import layout
from bokeh.plotting import figure
from datetime import datetime
from math import radians #rotate axis ticks
import numpy as np

# Create Figure
p = figure(x_axis_type="datetime", width=900, height=350)

# Generate Data
#IMPORTING LIBRARY
import requests
import json
import pandas as pd
import sqlite3 as db

#AREA EXTENT COORDINATE WGS4
lon_min,lat_min=-125.974,30.038
lon_max,lat_max=-68.748,52.214

#REST API QUERY
user_name='mouma'
password='momo19800712'
url_data='https://'+user_name+':'+password+'@opensky-network.org/api/states/all?'+'lamin='+str(lat_min)+'&lomin='+str(lon_min)+'&lamax='+str(lat_max)+'&lomax='+str(lon_max)





"""
value = flight_df['callsign']
#print(value)


for p in value:
    print(p)



df = pd.DataFrame(flight_df['velocity'], index=flight_df['callsign'], columns=['velocity'])
#print(df)
print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
print(df.loc[df.index[0], 'velocity'])


value1 = flight_df[['callsign','velocity']]
print(value1)
#for p in value1:
#    for a in value1['velocity']:
#        value1.rename(p:'a')
print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
value1.set_index('callsign')
print(value1.loc[value1.index[0], 'velocity'])
#print(value1)
#print(    value1.loc[0]        )

#value1 = value1.rename({0:"Témoin"})


#value1 = value1.rename({0:"Témoin",1:"Malade"})
#print(    value1.loc["Témoin"]        ) # Afficher la ligne Témoin
#print(    value1.loc[["Malade",2]]    )

#print(value1)
"""










def create_value():
    response=requests.get(url_data).json()
    #LOAD TO PANDAS DATAFRAME
    col_name=['icao24','callsign','origin_country','time_position','last_contact','long','lat','baro_altitude','on_ground','velocity',       
    'true_track','vertical_rate','sensors','geo_altitude','squawk','spi','position_source']
    flight_df=pd.DataFrame(response['states'])
    flight_df=flight_df.loc[:,0:16]
    flight_df.columns=col_name
    print(flight_df.loc[flight_df.index[0], 'velocity'])
    return flight_df.loc[flight_df.index[0], 'velocity']







# Create DAta Source
source = ColumnDataSource(dict(x=[], y=[]))

p.circle(x="x", y="y", color="firebrick", line_color="firbrick", source=source)
p.line(x="x", y="y", source=source)


# Create Periodic Function
def update():
    new_data = dict(x=[datetime.now()], y=[create_value()])
    source.stream(new_data , rollover=200)
    p.title.text="Now Streaming %s Data" % Select.value


def update_intermed(attrname, old, new):
    source.data=dict(x=[],y=[])
    update()


date_pattern = ["%Y-%m-%d\n%H:%M:%S"]


p.xaxis.major_label_orientation=radians(80)
p.xaxis.axis_label = "Date"
p.yaxis.axis_label = "Value"

#Create Selection Widget
options = [("stock1", "Stock One"), ("stock2", "Stock Two") ]
select = Select(title="Market Name", value="stock1", options=options)
select.on_change("value", update_intermed)

# Config Layout
lay_out = layout([[p], [select]])
curdoc().title = "Streaming Stock Data Example"
curdoc().add_root(lay_out)
curdoc().add_periodic_callback(update, 5000)


#///////////////////////////////////////////////////////////////




p1 = figure(x_axis_type="datetime", width=900, height=350)


def create_value1():
    response=requests.get(url_data).json()
    #LOAD TO PANDAS DATAFRAME
    col_name=['icao24','callsign','origin_country','time_position','last_contact','long','lat','baro_altitude','on_ground','velocity',       
    'true_track','vertical_rate','sensors','geo_altitude','squawk','spi','position_source']
    flight_df=pd.DataFrame(response['states'])
    flight_df=flight_df.loc[:,0:16]
    flight_df.columns=col_name
    print(flight_df.loc[flight_df.index[0], 'long'])
    return flight_df.loc[flight_df.index[0], 'long']







# Create DAta Source
source1 = ColumnDataSource(dict(x=[], y=[]))

p1.circle(x="x", y="y", color="firebrick", line_color="firbrick", source=source1)
p1.line(x="x", y="y", source=source1)


# Create Periodic Function
def update1():
    new_data1 = dict(x=[datetime.now()], y=[create_value1()])
    source1.stream(new_data1 , rollover=200)
    p1.title.text="Now Streaming %s Data" % Select.value


def update_intermed1(attrname, old, new):
    source1.data=dict(x=[],y=[])
    update1()


date_pattern1 = ["%Y-%m-%d\n%H:%M:%S"]


p1.xaxis.major_label_orientation=radians(80)
p1.xaxis.axis_label = "Date"
p1.yaxis.axis_label = "Value"

#Create Selection Widget
options = [("stock1", "Stock One"), ("stock2", "Stock Two") ]
select1 = Select(title="Market Name", value="stock1", options=options)
select1.on_change("value", update_intermed1)

# Config Layout
lay_out1 = layout([[p1], [select]])
curdoc().title = "Streaming Stock Data Example"
curdoc().add_root(lay_out1)
curdoc().add_periodic_callback(update1, 5000)


