#!/bin/bash

# Client 1
(
  for i in {1..50}; do
    echo "Client1_Message_$i"
    sleep 0.05
  done
  echo "quit"
) | nc localhost 4242 &

# Client 2
(
  for i in {1..50}; do
    echo "Client2_Message_$i"
    sleep 0.05
  done
) | nc localhost 4242 &

# Client 3
(
  for i in {1..50}; do
    echo "Client3_Message_$i"
    sleep 0.05
  done
) | nc localhost 4242 &

wait
