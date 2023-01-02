#!/bin/bash -ev

mkdir -p ~/.sumcoin
echo "rpcuser=username" >>~/.sumcoin/sumcoin.conf
echo "rpcpassword=`head -c 32 /dev/urandom | base64`" >>~/.sumcoin/sumcoin.conf

