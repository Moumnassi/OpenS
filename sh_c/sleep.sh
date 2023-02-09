#!/bin/bash

#Script pour faire l'acquisition chaque 300s du nombre d'avions sur paris

#AREA EXTENT COORDINATE WGS4
lon_min=1.974
lat_min=46.038
lon_max=3.748
lat_max=49.214

time=0

while sleep 300
do

curl -u "mouma:momo19800712" -s "https://username:password@opensky-network.org/api/states/all?lamin=$lat_min&lomin=$lon_min&lamax=$lat_max&lomax=$lon_max" | python3 -m json.tool > data_sleep.json


time=$((300/60+$time))

echo "Time : $time mn"	


nl=`wc -l data_sleep.json | cut -d ' ' -f 1`
nl=$(($nl-3))

rm test_sleep.txt

for (( i=1 ; i < 2000 ; i++ ))
do

var=$((4+17*$i+$i*2-2))
#echo " nl   : $nl "
#echo $var


if [ $var -lt $nl ]
then
	head -$var data.json | tail -17 | tr -d '[:space:]' |tr ',' '\t' >> test_sleep.txt
	echo '\n' >> test_sleep.txt
else
	break
fi

done

nl_test_sleep=`wc -l test_sleep.txt | cut -d ' ' -f 1`

echo "insert into flight (Time, Time_in_min ,Number_of_flights) values (datetime('now','localtime'), $time ,$nl_test_sleep)"| sqlite3 flight_sleep.db

done





   
   







