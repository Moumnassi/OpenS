#!/bin/bash

echo "Quelques variables multi Shell"




#AREA EXTENT COORDINATE WGS4
lon_min=1.974
lat_min=46.038
lon_max=3.748
lat_max=49.214


time=0

while sleep 10
do

curl -u "mouma:momo19800712" -s "https://username:password@opensky-network.org/api/states/all?lamin=$lat_min&lomin=$lon_min&lamax=$lat_max&lomax=$lon_max" | python3 -m json.tool > data.json


time=$((60/60+$time))

echo "Time : $time mn"	


nl=`wc -l data.json | cut -d ' ' -f 1`
nl=$(($nl-3))

rm test.txt

for (( i=1 ; i < 2000 ; i++ ))
do

var=$((4+17*$i+$i*2-2))
#echo " nl   : $nl "
#echo $var

if [ $var -lt $nl ]
then
	head -$var data.json | tail -17 | tr -d '[:space:]' |tr ',' '\t' >> test.txt
	echo '\n' >> test.txt
else
	break
fi

done

CODE="AFR14DF"
cut -f 2 test.txt > aide_1.txt
ligne=$(grep -n $CODE aide_1.txt | cut -d: -f1)
echo "ligne   : $ligne"
cut -f 10 test.txt > aide_2.txt
velocity=`head -$ligne aide_2.txt | tail -1`



echo "insert into flight (Time,Time_in_min, velocity) values (datetime('now','localtime'), $time , $velocity)"| sqlite3 flight.db

gcc -o main main.c -lgd -lpng -lz -ljpeg -lfreetype -lm -lsqlite3
./main


done

#./sql_code.py

