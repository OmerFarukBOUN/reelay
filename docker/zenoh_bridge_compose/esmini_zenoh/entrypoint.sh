#!/bin/bash
/zenoh-bridge/apps/zenoh_bridge/build/esmini_zenoh &

sleep 5

echo "Current directory: $(pwd)"

/zenoh-bridge/apps/zenoh_bridge/build/zenoh_test_sub &

sleep 5

./bin/esmini --window 60 60 800 400 --osc ./resources/xosc/cut-in.xosc --fixed_timestep 0.05 --osi_receiver_ip &

wait &&

sleep 1 

python3 /zenoh-bridge/data.py
