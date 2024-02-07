#!/bin/bash

mosquitto_sub -h localhost -p 44444 --insecure --cafile ../certificate/ca.crt --cert ../certificate/client.crt --key ../certificate/client.key -u "user02" -P "password2" -t test_topic