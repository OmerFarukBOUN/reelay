#!/bin/bash
/pub_sub_reelay/apps/zenoh_bridge/build/zenoh_bridge &

sleep 5

/pub_sub_reelay/apps/zenoh_bridge/build/zenoh_test_sub &

sleep 1

/pub_sub_reelay/apps/zenoh_bridge/build/zenoh_test_pub_esmini


