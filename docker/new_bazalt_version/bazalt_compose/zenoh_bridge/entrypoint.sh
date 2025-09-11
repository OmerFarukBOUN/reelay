#!/bin/bash
/zenoh-bridge/apps/zenoh_bridge/build/zenoh_bridge &

sleep 5

/zenoh-bridge/apps/zenoh_bridge/build/zenoh_test_sub &

sleep 1

/zenoh-bridge/apps/zenoh_bridge/build/zenoh_test_pub_esmini


