#!/bin/bash

counter=0

while [  $counter -eq 0 ]; do

    if [ -f new_binary ]; then
        echo "prepare new version"
        mv new_binary shp_client
        chmod +x shp_client
    fi

    ./shp_client
    if [ "$?" -ne "0" ]; then
        echo "restart requested"
    else
        echo "exit"
        counter=1
    fi

done
