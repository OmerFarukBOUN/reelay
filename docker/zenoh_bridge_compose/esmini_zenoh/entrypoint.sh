#!/bin/bash
/zenoh-bridge/apps/zenoh_bridge/build/esmini_zenoh &

./bin/esmini --headless --osc ./resources/xosc/acc-test.xosc --fixed_timestep 0.05 --osi_reciever_ip &

wait
