

### !! NOTICE TO ANY EXCHANGE OR DEVELOPERS ETC -  THIS COIN IS INDEX BASED AND YOU SHOULD CONTACT US WITH QUESTIONS!!

OBJECT IS **INDEX BASED**, ALGO BASED COIN FOR USE IN **ATM'S** 

### See General Bytes Twitter for example:
https://twitter.com/generalbytes/status/1098174237163077633

FOR QUESTIONS, AUTHORIZATIONS, CONTACT : ATTN DEVS @ Support@SumcoinIndex.com

### BATM Setup instructions:
https://github.com/sumcoinlabs/sumcoin/wiki/Sumcoin-Core-(sumcoind)-BATM-Configuration

//---

## Sumcoin integration/staging tree

CONTACT US: Support@SumcoinIndex.com Twitter https://twitter.com/SumCoinIndex Facebook @SumCoinIndex

Main Website https://SumcoinIndex.com

### Email 
Support@SumcoinIndex.com



# Mining

Mining Pool Website http://sumcoinpool.org

## Stratum connect info 
stratum+tcp://sumcoinpool.org:3340

Pricing Info http://sumcoinprice.com/

SUM Block Explorer http://sumexplorer.com

TODO - Add charts from db to sumcoinprice.com

Download versions & other Information: http://Sumcoin.cash

## PRICE Data

Exchange rate Pricing JSON API : 

### Integer, Rate, Symbol :

https://sumcoinindex.com/rates/price.json

AND

### Last buy, last price, last sell rate data
https://sumcoinindex.com/rates/price2.json



# To Buy, Sell or Trade (Coin to Coin)
HTTPS://SumcoinTrade.com 


## Mining Hardware

### Sumcoin Algo is Scrypt

CPU, GPU Use CPU Miner by Pooler (see our link to cpu miner - minerd - in releases)

AMD GPU Cards SGMINER for AMD Cards : https://bit.ly/2k6zchT

Moonlander 2 USB Asic Miner : https://www.futurebit.io/


Copyright (c) 2009-2014 Bitcoin Developers Copyright (c) 2011-2017 Litecoin Developers Copyright (c) 2017-2018 Sumcoin Developers Copyright (c) 2017-2018 Crypto Cloud Inc

## What is Sumcoin?

SumCoin is a cryptographic blockchain using scrypt proof-of-work algorithm. Sumcoin tracks all coins in real time and is an aggregate or "sum" of all top 100 coins by market capitalization. It is for those who want to gain maximum exposure to the crypto space but may only want to hold one coin for simplicity, which can reduce risk factors.

## Times:

1.25 minute block targets

subsidy halves in 1M blocks (~2 years)

~100 million total coins

50 coins per block

3619 blocks to retarget difficulty (pi in days)

For more information, as well as an immediately useable, binary version of the Sumcoin client sofware, see https://github.com/sumcoinlabs/sumcoin/releases

### License

Sumcoin is released under the terms of the MIT license. See COPYING for more information or see http://opensource.org/licenses/MIT.

### Development process

Developers work in their own trees, then submit pull requests when they think their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the Sumcoin development team members simply pulls it.

If it is a more complicated or potentially controversial change, then the patch submitter will be asked to start a discussion with the devs and community.

The patch will be accepted if there is broad consensus that it is a good thing. Developers should expect to rework and resubmit patches if the code doesn't match the project's coding conventions (see doc/coding.txt) or are controversial.

The master branch is regularly built and tested, but is not guaranteed to be completely stable. Tags are created regularly to indicate new official, stable release versions of Sumcoin.

### Build Dependencies/instructions: (Also see Sumcoin Wiki for the same info)
 
Dependencies:
 
sudo apt-get install git
 
sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils
 
sudo apt-get install libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev
 
sudo apt-get install libboost-all-dev
 
sudo apt-get install software-properties-common
 
sudo add-apt-repository ppa:bitcoin/bitcoin
 
sudo apt-get update
 
sudo apt-get install libdb4.8-dev libdb4.8++-dev
 
sudo apt-get install libminiupnpc-dev
 
sudo apt-get install libzmq3-dev
 
sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler
 
sudo apt-get install libqt4-dev libprotobuf-dev protobuf-compiler

### Next, Clone the project
``` 
git clone https://github.com/sumcoinlabs/sumcoin.git
```
### Change directories into Sumcoin
```
cd sumcoin
```
### From the Sumcoin Directory, run each command
```
./autogen.sh
```
Next,
```
./configure
```
last, make the executables
```
make
```

## SPECIAL NOTE if using only 1 GB RAM - File Swap info (if needed):

Create swapfile using:
sudo fallocate -l 2G /swapfile  
sudo chmod 600 /swapfile 
sudo mkswap /swapfile
sudo swapon /swapfile

### Make:

Go to sumcoin directory and run the following:
./autogen.sh
./configure --disable-tests --without-gui
 make

It will then start compiling and take a while. 

### IF you needed swapfile - BE sure to turn it off again:

When it is done, turn off the swapfile with:
sudo swapoff /swapfile  

### Run: 

After this it's ready to run. The executable will be in sumcoin/src. Run with:
./sumcoind -server -daemon

### Testing

Testing and code review is the bottleneck for development; we get more pull requests than we can review and test. Please be patient and help out, and remember this is a security-critical project where any mistake might cost people lots of money.

Automated Testing
Developers are strongly encouraged to write unit tests for new code, and to submit new unit tests for old code.

Unit tests for the core code are in src/test/. To compile and run them:

cd src; make -f makefile.unix test
Unit tests for the GUI code are in src/qt/test/. To compile and run them:

qmake BITCOIN_QT_TEST=1 -o Makefile.test bitcoin-qt.pro
make -f Makefile.test
./sumcoin-qt_test
