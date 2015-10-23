#!/bin/bash

sig_level_str=`cat /proc/net/wireless | grep wlp2s0b1 | sed 's/|/ /g' |awk '{print $4}'`
sig_level=${sig_level_str//-/}
sig_level=${sig_level//./}

if [ "$sig_level" == "" ]
then
sig_level=100
fi

echo $sig_level
