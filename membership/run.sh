#!/bin/bash

# initiate some members
./build/swim-protocol --host localhost --port 9001 --chost 127.0.0.1 --cport 9000 > process1.log 2>&1 &
./build/swim-protocol --host localhost --port 9002 --chost 127.0.0.1 --cport 9000 > process2.log 2>&1 &
# ./build/swim-protocol --host localhost --port 9003 --chost localhost --cport 9000 > process3.log 2>&1 &
# ./build/swim-protocol --host localhost --port 9004 --chost localhost --cport 9000 > process4.log 2>&1 &

# start the coordinator
./build/swim-protocol --host localhost --port 9000 --coordinator | tee coordinator.log