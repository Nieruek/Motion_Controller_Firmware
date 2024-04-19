#!/bin/bash

DEF_DEV="/dev/ttyACM0"
DEF_TIMEOUT="1"
INC_TIME="1"
MOVE_CMD="set 1120"
POS_1="90"
POS_2="-90"

move_func() {
   echo "$MOVE_CMD $POS_1 " > "$DEF_DEV"
   echo "$MOVE_CMD $POS_1 , sleep $DEF_TIMEOUT"
   sleep $DEF_TIMEOUT
   echo "$MOVE_CMD $POS_2 "  > "$DEF_DEV"
   echo "$MOVE_CMD $POS_2 , sleep $DEF_TIMEOUT"
   sleep $DEF_TIMEOUT	
}
# Check for input device 

if [ -z "$1" ]; then
  echo "No argument 1 provided. Using: $DEF_DEV"
else
   if [ "$1" == "loop" ];then
   	DEF_TIMEOUT="1"
   	POS_1="720"
   	POS_2="-720"
   	while [ 0 ]; do
           move_func
           DEF_TIMEOUT="$(("$DEF_TIMEOUT"+"$INC_TIME"))"
           if [ $DEF_TIMEOUT -gt "5" ];then
           	DEF_TIMEOUT="1"
           fi
        done
   else 
       DEF_DEV=$1
      echo "Device to use: $1"
   fi
fi

if [ -z "$2" ]; then
  echo "No argument 2 provided. Using timeout: $DEF_TIMEOUT"
else
DEF_TIMEOUT=$2
  echo "Timeout to use: $2"
fi

if [ -z "$3" ]; then
  echo "No argument 2 provided. Using angle: $POS_1"
else
POS_1=$3
POS_2=-$3
  echo "Angle to use: $3"
fi

while [ 0 ]; do
   move_func
done


