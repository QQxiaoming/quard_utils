#!/bin/bash

# Convert 4byte hex to word
# Usage: byte2word 0x12 0x34 0x56 0x78
# return: 0x12345678
byte2word() {
    local byte1=$1
    local byte2=$2
    local byte3=$3
    local byte4=$4
    local ret="0x$(printf "%02X%02X%02X%02X" $byte1 $byte2 $byte3 $byte4)"
    echo $ret "(" $byte1 $byte2 $byte3 $byte4 ")"
}

# Convert file to hex
# Usage: file2hex fw.bin
# return: 0x12 0x34 0x56 0x78 ...
file2hex() {
    local file=$1
    local ret=$(hexdump -v -e '1/1 "0x%02X "' $file)
    echo $ret
}

# Cut data
# Usage: cutdata $data $size $index
# Usage: cutdata "0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88 0x99" 4 1
# return: 0x55 0x66 0x77 0x88
cutdata() {
    local data=$1
    local size=$2
    local index=$3
    local data_len=$(echo $data | wc -w)
    if [ $((index*size+size)) -gt $data_len ]; then
        local ret=$(echo $data | cut -d' ' -f$((index*size+1))-)
    else
        local ret=$(echo $data | cut -d' ' -f$((index*size+1))-$((index*size+size)))
    fi
    echo $ret
}
