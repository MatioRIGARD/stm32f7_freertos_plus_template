#!/bin/bash

# remove old gen
# rm *.crt *.key *.srl *.csr

## Certificate Authority (CA)
# gen certification authority private key
openssl genrsa -out ca.key 4096
# gen certificate of certification authority
openssl req -new -x509 -days 3650 -key ca.key -out ca.crt

## for MQTT client
# client private key
openssl genrsa -out client.key 2048
# client signature request (csr)
openssl req -new -key client.key -out client.csr
# sign csr with authority to gen client certificate
openssl x509 -req -days 365 -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt

## for MQTT broker
# server private key
openssl genrsa -out server.key 2048
# csr for server
openssl req -new -key server.key -out server.csr
# sign csr of server with ca
openssl x509 -req -days 365 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt


