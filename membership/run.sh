#!/bin/bash

# start the coordinator
./build/swim-protocol --host localhost --port 9000 --coordinator > process0.log 2>&1 &

# make sure the coordinator is up and running before anything else starts
sleep 1

# initiate some members
./build/swim-protocol --host localhost --port 9001 --chost localhost --cport 9000 > process1.log 2>&1 &
# ./build/swim-protocol --host localhost --port 9002 --chost process0 --cport 9000 > process2.log 2>&1 &
# ./build/swim-protocol --host localhost --port 9003 --chost process0 --cport 9000 > process3.log 2>&1 &
# ./build/swim-protocol --host localhost --port 9004 --chost process0 --cport 9000 > process4.log 2>&1 &
