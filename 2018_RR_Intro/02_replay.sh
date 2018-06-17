#!/bin/bash

rc=1 ; while [ $rc -ne 0 ] ; 
do 
    rr record ./02_replay | grep "NANA" 
    rc=$?
    echo $rc
done
