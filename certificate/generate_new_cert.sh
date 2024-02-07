#!/bin/bash

# remove old gen
rm *.crt *.key *.srl *.csr

## Certificate Authority (CA)
# gen certification authority private key
echo "> openssl genrsa -out ca.key 2048"
openssl genrsa -out ca.key 2048
# gen certificate of certification authority
echo "> openssl req -new -x509 -config openssl_ca.conf -key ca.key -out ca.crt"
openssl req -new -x509 -config openssl_ca.conf -key ca.key -out ca.crt

## for MQTT client
# client private key
echo "> openssl genrsa -out client.key 2048"
openssl genrsa -out client.key 2048
# client signature request (csr)
echo "> openssl req -new -config openssl_server.conf -key client.key -out client.csr"
openssl req -new -config openssl_server.conf -key client.key -out client.csr
# sign csr with authority to gen client certificate
echo "> openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt"
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt

## for MQTT broker
# server private key
echo "> openssl genrsa -out server.key 2048"
openssl genrsa -out server.key 2048
# csr for server
echo "> openssl req -new -config openssl_client.conf -key server.key -out server.csr"
openssl req -new -config openssl_client.conf -key server.key -out server.csr
# sign csr of server with ca
echo "> openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt"
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt

# change rights on key files
chmod 644 *.key
