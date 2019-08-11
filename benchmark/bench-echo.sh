#!/bin/bash

set -e

echo ""
echo "--- BENCH ECHO START ---"
echo ""

cd $(dirname "${BASH_SOURCE[0]}")
function cleanup {
    echo "--- BENCH ECHO DONE ---"
}
trap cleanup EXIT

mkdir -p bin

function gobench {
    echo "--- $1 - $2-$3-"
    GOMAXPROCS=1 $2 --port $3 &
    sleep 1
    echo "*** 50 connections, 10 seconds, 6 byte packets"
    nl=$'\r\n'
    tcpkali --workers 1 -c 100 -T 10s -m "PING$nl" 127.0.0.1:$2
    echo "--- DONE ---"
    echo ""
}

gobench "reimu"  3099
pkill  reimu
#gobench "reimu_kqueue"  2030
#pkill  reimu_kqueues
