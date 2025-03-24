#!/bin/bash
/zenoh-bridge/apps/zenoh_bridge/build/esmini_zenoh &

sleep 5

echo "Current directory: $(pwd)"

./bin/esmini --headless --osc ./resources/xosc/acc-test.xosc --fixed_timestep 0.05 --osi_receiver_ip &

wait
