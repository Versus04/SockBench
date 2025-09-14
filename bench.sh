#!/bin/bash

# Ensure server is already running in another terminal
# This script will run multiple client tests and log results

OUTPUT="results.csv"

# Remove old results.csv if it exists
if [ -f "$OUTPUT" ]; then
    rm "$OUTPUT"
fi

# Define test cases: message_size num_messages
TESTS=(
    "32 10000"
    "64 10000"
    "128 10000"
    "256 10000"
    "512 10000"
    "1024 10000"
    "2048 10000"
    "4096 10000"
    "8192 5000"
    "16384 2000"
    "32768 1000"
    "65536 1000"
)

# Run each test
for test in "${TESTS[@]}"; do
    ./server &      # start server in background
    SERVER_PID=$!
    sleep 1         # give it a second to start

    ./client $test

    kill $SERVER_PID
    sleep 1
done


echo "All benchmarks completed. Results saved in $OUTPUT"
s
