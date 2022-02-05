#!/bin/bash -ev

mkdir -p ~/.sumcash
echo "rpcuser=username" >>~/.sumcash/sumcash.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.sumcash/sumcash.conf

