#!/bin/bash

mosquitto_pub -h localhost -p 44444 --insecure --cafile ../certificate/ca.crt --cert ../certificate/client.crt --key ../certificate/client.key -u user01 -P password -t test_topic -m "new message"
