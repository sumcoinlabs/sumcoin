# Sumcoin Official Development Repo


[![Continuous Integration](https://github.com/sumcoinlabs/sumcoin/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/sumcoinlabs/sumcoin/actions/workflows/build.yml)

### What is Sumcoin?
[Sumcoin](https://sumcoin.org) (abbreviated SUM), is a [cryptocurrency](https://cryptocurrency.fandom.com/wiki/Sumcoin) design of [proof-of-stake consensus](https://sumcoinindex.com/white-paper.html) as a security model minting system. Sumcoin is based on [Bitcoin](https://bitcoin.org), while introducing many important innovations to cryptocurrency field including new security model, energy efficiency, better minting model and more adaptive response to rapid change in network computation power.

<html>
<table>
<tbody>
 <tr>
  <td>Twitter</td>
  <td>
   <a href="https://twitter.com/sumcoinindex" rel="nofollow">
    <img src="https://camo.githubusercontent.com/d4ddbb597e350f99050aeda320d0278687e416e3/68747470733a2f2f696d672e736869656c64732e696f2f747769747465722f666f6c6c6f772f626f6b65682e7376673f7374796c653d736f6369616c266c6162656c3d466f6c6c6f77" alt="Follow Sumcoin on Twitter" data-canonical-src="https://img.shields.io/twitter/follow/bokeh.svg?style=social&amp;label=Follow" style="max-width:100%;">
    </a>
  </td>
 </tr>
</tbody
</table>
 </html>

Sumcoin is an Index Based Coin
==============================

## !! NOTICE TO ANY EXCHANGE OR DEVELOPERS  -  SUMCOIN IS INDEX BASED - READ THIS BEFORE INTEGRATING!!

### OBJECT IS **INDEX BASED**

# What is Sumcoin?

Sumcoin is a cryptographic blockchain using scrypt proof-of-work algorithm. Sumcoin tracks all coins in real time and its value is an aggregate or "SUM" of all top 100 coins by market capitalization. This is done using algorithms that track and calculate the market, without human interaction.  It is for those who want to gain maximum exposure to the crypto space but may only want to hold one coin for simplicity, which can also reduce risk factors.


Sumcoin Core integration/staging tree [SUM, 𝚺]
==============================================


![image](https://user-images.githubusercontent.com/19527587/63079030-868d0100-befa-11e9-8d0d-d1cb079772f5.png)


# **Use Rates API for your APP**

# SUM / USD
## Rate API Sources

* https://rates.sumcoinindex.com/api/rates
  * https://sumcoinindex.com/rates/price2.json
  * https://sumcoinindex.com/rates/marketcap.json

* https://rates.slicewallet.org/api/rates

# SUM / BTC / USDT
## Rate API Sources

* https://sumcoinindex.com/rates/sum_btc.json
* https://sumcoinindex.com/rates/SUM_USDT.json

# TX fees api
## Fee sum/kb API
* https://rates.sumcoinindex.com/api/fee_per_kb
* https://rates.slicewallet.org/api/fee_per_kb




### Basic principals to adhere to Index Rate with online exchange
<ol>
 <li>Traders/members should not be able to place Buy orders greater than the Index price</li>

<li>Traders/members should not be able to place Sell orders less than the Index price</li>

<li>If existing orders are present they should be automatically canceled as Index price grows more than a reasonable % amount. </li>

<li>
Each exchange should determine what quantifies what a reasonable % is.  While Sumcoin devs feel <strong>no more</strong> than <strong>1% +/-</strong> to the <strong>Index rate</strong> avoids arbitrage,  this will be left to the exchanges to decide the exact tolerances of.
</li>
</ol>

# Specifications
--------------
Specification | Descriptor
------------- | ----------
Ticker Symbol                  | SUM
Algorithm                      | SHA-256
Maxiumum Supply                | 200,000,000 + POS reward of 3% annual
SegWit                         | Activate in future
BIP58 Prefix Public            | 0x04, 0x88, 0xB4, 0x1C
BIP58 Prefix Private           | 0x04, 0x88, 0xAB, 0xE6
Magic Number                   | 0xf1e6f9a2
BIP32 public:                  | 0xF588B21F           
BIP32 private:                 | 0xF588ADE5
pubKeyHash:                    | 0x3F
scriptHash:                    | 0x7d
wif:                           | 0xbb
Mainnet RPC Port               | 3332
Mainnet P2P Port               | 3333
Electrum Mainnet RPC Port      | 53332
Electrum Mainnet P2P Port      | 53333
Testnet RPC Port               | 13332
Testnet P2P Port               | 13333
regtest port                   | 19444
Block Time                     | 60 Seconds
Coinbase Transaction Maturity  | 100 Blocks
Confirmation                   | 6 Blocks
Difficulty Adjustment Interval | 2880 Blocks
Protocol Support               | IPV4, IPV6, TOR, I2P


# Social and resources
https://sumcoin.org/
https://sumcoinwallet.org/
https://twitter.com/Sumcoinindex
Fandom: https://cryptocurrency.fandom.com/wiki/Sumcoin

## Chat
* Discord: https://discordapp.com/invite/ffJT5s8
* Telegram: https://t.me/sumcoins
* Twitter: https://twitter.com/SumcoinIndex
* Explorer 2: https://sumcoinexplorer.com



## Maps
* Nodes:  https://www.sumnode.io/
* Note:  Dependancies may not be working for map display as of Sept 2020

![Map](https://user-images.githubusercontent.com/51257210/66169341-4ba37180-e5fd-11e9-9069-24eae46dc2b6.png)



## BATM Setup instructions:
https://github.com/sumcoinlabs/sumcoin/wiki/Sumcoin-Core-(sumcoind)-BATM-Configuration

//---

## Sumcoin integration/staging tree

Developers work in their own trees, then submit pull requests when they think their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the Sumcoin development team members simply pulls it.

If it is a more complicated or potentially controversial change, then the patch submitter will be asked to start a discussion with the devs and community.

The patch will be accepted if there is broad consensus that it is a good thing. Developers should expect to rework and resubmit patches if the code doesn't match the project's coding conventions (see doc/coding.txt) or are controversial.

The master branch is regularly built and tested, but is not guaranteed to be completely stable. Tags are created regularly to indicate new official, stable release versions of Sumcoin.


## Issues

For help:  Open an Issue



# Block Explorers

* SUM Block Explorer https://sumcoinexplorer.com

Download versions & other Information: https://sumcoin.org



## Mining Hardware

### Sumcoin Algo is Sha-256

CPU, Proof of Stake


Copyright (c) 2009-2014 Bitcoin Developers Copyright (c) 2017-2023 Sumcoin Developers Copyright / Sumcoinlabs / Squawk LLC



### Development process

Developers work in their own trees, then submit pull requests when they think their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the Sumcoin development team members simply pulls it.

If it is a more complicated or potentially controversial change, then the patch submitter will be asked to start a discussion with the devs and community.

The patch will be accepted if there is broad consensus that it is a good thing. Developers should expect to rework and resubmit patches if the code doesn't match the project's coding conventions (see doc/coding.txt) or are controversial.

The master branch is regularly built and tested, but is not guaranteed to be completely stable. Tags are created regularly to indicate new official, stable release versions of Sumcoin.

## Linux Build Dependencies/instructions: (Also see Sumcoin Wiki for the same info)

## SPECIAL NOTE if using only 1 GB RAM - File Swap info (if needed):

Create swapfile using (copy and paste in all 4 for an easy life):

```
sudo fallocate -l 2G /swapfile;
sudo chmod 600 /swapfile;
sudo mkswap /swapfile;
sudo swapon /swapfile;
```
### Make it persistent 
```
sudo nano /etc/fstab
```
### add line:
/swapfile  swap  swap  defaults  0  0


### Dependencies (paste all 3 seperately):

```
sudo apt-get update
sudo apt-get install git
sudo apt-get install -y build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils
sudo apt-get install -y libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev
sudo apt-get install -y libboost-all-dev
sudo apt-get install -y software-properties-common

sudo add-apt-repository ppa:bitcoin/bitcoin

sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev
sudo apt-get install -y libminiupnpc-dev
sudo apt-get install -y libzmq3-dev
sudo apt-get install -y libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler
sudo apt-get install -y libqt4-dev libprotobuf-dev protobuf-compiler
```
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

Next choose configuration option
* Full Build
```
./configure
```

* Build with GUI without tests (faster)
```
./configure --disable-tests
```

* Build without tests or GUI
```
./configure --disable-tests --disable-bench --without-gui
```

# Make

### Last, make the executables (This could take a bit)

```
make
```
*It will then start compiling and take a while.*



### IF using a 1 GB Droplet be sure to turn OFF after you 'make'

```
sudo swapoff /swapfile
```


# Run:

**After this it's ready to run.**  The executable will be in sumcoin/src. Run with:

```
./sumcoind -server -daemon
```

## Setup a configuration file to run as a node

### Stop the server from sumcoin/src:

```
./sumcoin-cli stop
```

## Navigate to sumcoin data dir from home dir

```
cd .sumcoin
```

```
touch sumcoin.conf
nano sumcoin.conf
```

## Paste the following **make your own user/password***

```
server=1
daemon=1
rpcuser=YOURuserNAME
rpcpassword=YOURpassword
```

## Restart Sumcoin.   You will now be running as a node each time you start

```
cd sumcoin/src/:./sumcoind -server -daemon
```

**With GUI** - Works for Linux terminal

```
cd sumcoin/src/qt/:./sumcoin-qt
```

## Configure your settings

* 1. navigate to hidden folders
* 2. create sumcoin.conf file
* 3. inside, set the following parameters from the list below



### Testing

Testing and code review is the bottleneck for development; we get more pull requests than we can review and test. Please be patient and help out, and remember this is a security-critical project where any mistake might cost people lots of money.

Automated Testing
Developers are strongly encouraged to write unit tests for new code, and to submit new unit tests for old code.

Unit tests for the core code are in src/test/. To compile and run them:

cd src; make -f makefile.unix test
Unit tests for the GUI code are in src/qt/test/. To compile and run them:

qmake SUMCOIN_QT_TEST=1 -o Makefile.test sumcoin-qt.pro
make -f Makefile.test
./sumcoin-qt_test




# ATM Setup


## How to Setup Sumcoin For General Bytes ATM's



## Sumcoin Core (sumcoind) Configuration

The following example is based on the presumption that you have completed the prior steps of CAS installation on a hosted Internet server running Ubuntu 16.04 LTS, or higher, that the server is online, and the BATM has been successfully installed, configured, and tested for proper connectivity to the CAS.

A further presumption is that the sumcoind daemon has been installed, configured, and tested for proper use. This guide is merely designed to help you setup a working sumcoind daemon with a working CAS.

This is a step-by-step guide for adding sumcoind as a Hot Wallet Buy source. This is only an example and not to be construed as an endorsement for any particular exchange or wallet.

### This has been tested on sumcoin Core v 0.20.2


### First, open a terminal window and login to your server as root.

Create an RPC token for CAS to communicate with sumcoind:
The RPC token is designed to eliminate the need for hard-coded passwords in configuration and script files. You will receive a password here ONCE. This password is required for CAS; it's your “RPC Password”. You must save it during this step.

## Download rpcauth.py on GitHub  (all one line):
### Not required if you compile your own binaries

```
wget https://raw.githubusercontent.com/sumcoinlabs/sumcoin/master/share/rpcauth/rpcauth.py -O ~/.sumcoin/rpcauth.py
```

### Modify the file permissions to allow the script to execute:

```
chmod +x ~/.sumcoin/rpcauth.py
```
## Best Method
### Then finally run the RPC token generator that you just installed and enabled:
### Path,
root@root:~/sumcoin/share/rpcauth

```
./rpcauth.py AnyNameYouWantHere
```
## Example Result

root@root:~/sumcoin/share/rpcauth$ ./rpcauth.py sumcoinatm

**String to be appended to sumcoin.conf:**

rpcauth=AnyNameYouWantHere:aac8dlfjsdfkjds876dfpdsfjhlsdlkfasd8plkdjf8d8984lndlkdfjsdflkjsadfkfjkhlkjhdjdfh3b90e44ddca2
Your password:
T0Dx2VLjgikeTANOGosdifoIJH80Xn2E0Tcx-jdlh1GIcr8c=


## Save
The name you entered above, the resulting token, AND the generated password. You'll need it in the next step. The “AnyNameYouWantHere” is the “RPC User”, and the generated password is your “RPC Password” for CAS. The token is put in the sumcoin.conf file (as described below).

### A sample token is all one line and should look similiar to this:

AnyNameYouWantHere:aac8dlfjsdfkjds876dfpdsfjhlsdlkfasd8plkdjf8d8984lndlkdfjsdflkjsadfkfjkhlkjhdjdfh3b90e44ddca2

### The generated password may look like:
T0Dx2VLjgikeTANOGosdifoIJH80Xn2E0Tcx-jdlh1GIcr8c=

### Modify the sumcoin.conf configuration file:

While you might (under certain circumstances) might want to start the daemon exclusively from a command line, we urge you to change the following settings within sumcoind.conf and run sumcoind without parameters. Keep it simple!

## Locate the Sumcoin Core configuration file.

- sumcoin.conf in data dir -  

### Add (or modify) the following settings in the configuration file:

```
deprecatedrpc=accounts
deprecatedrpc=estimatefee
addresstype=legacy
server=1
daemon=1
rpcallowip=your_Server_IP_for_CAS
```

The RPC (Remote Procedure Call) uses "credentials" to communicate between programs. It's required, and can be a security issue if insecure. Depending on the RPC settings and your firewall, you might expose your wallet to the world - and that would be unwise. Research the subject before randomly changing any settings.

### Add your RPC user credentials:

```
rpcuser=AnyNameYouWantHere
rpcpassword=DontUseThis!YouHaveBeenWarned!
-or (preferred)-
rpcauth=AnyNameYouWantHere:e5305a4d7e2c760b196882d2896f77$c5b5e7f6970f7c0544244189f45622887642dca2f36761bcecc1d8055fe8416d
```

### Your end result should look like this Example in sumcoin.conf (Tested v0.16.1)
```
addresstype=legacy
deprecatedrpc=accounts
deprecatedrpc=estimatefee
server=1
daemon=1
rpcallowip=your_Server_IP_for_CAS
rpcauth=WalletName:bbc8fHC2jkeyojdlnlfnd89c1d61cee0aa70aa947e060617fd9231fac102b25cchh3jwls7bBb90e44ddca2
```
### Use either the top method or bottom (preferred) method.

If using the first/top method, the rpcpassword is whatever you want it to be.      
The “rpcuser/rpcpassword” method is deprecated because it is considered a security risk.
If using the second/bottom method:      
the "rpcauth" is all one line,

Replace the “rpcauth” info with the token generated in the previous step.
RPC tokens (like this) are the preferred method of RPC authorization.



### Start the sumcoind daemon (background process):

Once you have modified sumcoin.conf, **(re)start the daemon:**

root@root:./sumcoind -server -daemon

### It may take several minutes for Sumcoin Core to completely start.

Sumcoin Core and CAS will not be able to communicate until Sumcoin Core has "warmed up". It may take a few minutes - or longer.

Ascertain that you have a default address. Use:
```
sumcoin-cli listaccounts
```
### One of three things will be displayed:
"Sumcoin Core is still "warming up". Give it a few minutes and try again."

"Empty brackets {} indicate that no address exists. You must create one."
_Anything between the brackets means you can safely skip to the next part._

If you get empty brackets (result #2), then create a default Wallet Address and Account name by running:
```
sumcoin-cli getnewaddress "makeUpWalletNameBetweenQuotes"
```
### You can safely skip this if you are CERTAIN one has already been created.
CAS will NOT work with Sumcoin Core unless at least one address exists!

NOTE: sumcoind will take several hours (or longer) to completely synchronize with the Sumcoin network. CAS will NOT work with sumcoind until the synchronization is COMPLETE.


## Configure CAS to communicate with your sumcoin Core wallet:

### Assemble the required information for CAS:

User: this is the “RPC User” you created earlier. For this example we'll just say it's “rpcuser”.
Password: is the “RPCPassword” created by rpcauth.py earlier, or the one you made up (if you don't implement a token).  For this example, we'll make the password just plain “rpcpw”.

### Accountname:
this is the account name of your wallet. An account name is neither required nor suggested. Account usage is deprecated in sumcoind, however to view any existing accounts, type:
```
sumcoin-cli listaccounts
```
This parameter may be safely omitted if only a default account exists ("").
If you choose to specify an account, select one named in the list.
Whether using the default ("") or not, we'll refer to it as "walletname" in the examples below.



### Next, add a new crypto-currency type to CAS:
* Enter the “Crypto Settings” menu in the left-hand column.
* Click on the “+ ADD” button.
* Create a suitable description, i.e. “sumcoind SUM”.
* Select “SUM” for the required Crypto Currency* setting.
* Set Configuration Cash Currency* to“USD” (or whatever your local currency might be).
* Set Buy Rate Source to whatever source you prefer.
* Set Minimum Cash Amount PerTransaction* to zero – this enables any sale.
* Select your Hot Wallet Buy source to “sumcoin Core - sumcoind”.
* Enter the Parameters for the Hot Wallet Buy source as determined in the previous step:

### Example (typical):

“http:walletName:rpcpw:DaemonIP:3332”

### Example with a specific wallet:

“http:rpcuser:rpcpw:DaemonIP:3332:walletname”

## Click on “SUBMIT” to save your settings.

## Finally, test it out by selecting Test Hot Wallet Buy from the RUN SUM SETTINGS TEST.

### Presuming everything was entered correctly, you should receive confirmation of success!
You should also now configure the Hot Wallet Sell using the same parameters if you implement it on a BATMThree (or other 2-way).

      ##     You're done!


### Sumcoin Resources
* Client and Source:
[Client Binaries](https://github.com/sumcoinlabs/sumcoin/releases),
[Source Code](https://github.com/sumcoinlabs/sumcoin)
* Documentation: [Sumcoin Docs](https://sumcoin.org)
* Help:
[Forum](https://sumcoin.org),
[Intro & Important Links](https://talk.sumcoin.org/t/what-is-sumcoin-intro-important-links/2889)
[Telegram Chat](https://t.me/sumcoins)

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test. Please be patient and help out, and
remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write unit tests for new code, and to submit new unit tests for old code.

Unit tests can be compiled and run (assuming they weren't disabled in configure) with:
  make check

### Manual Quality Assurance (QA) Testing

Large changes should have a test plan, and should be tested by somebody other than the developer who wrote the code.

* Developers work in their own forks, then submit pull requests when they think their feature or bug fix is ready.
* If it is a simple/trivial/non-controversial change, then one of the development team members simply pulls it.
* If it is a more complicated or potentially controversial change, then the change may be discussed in the pull request, or the requester may be asked to start a discussion in the [Sumcoin Forum](https://talk.sumcoin.org) for a broader community discussion.
* The patch will be accepted if there is broad consensus that it is a good thing. Developers should expect to rework and resubmit patches if they don't match the project's coding conventions (see coding.txt) or are controversial.
* From time to time a pull request will become outdated. If this occurs, and the pull is no longer automatically mergeable; a comment on the pull will be used to issue a warning of closure.  Pull requests closed in this manner will have their corresponding issue labeled 'stagnant'.
* For development ideas and help see [here](https://talk.sumcoin.org/c/protocol).

## Branches:

### develop (all pull requests should go here)
The develop branch is used by developers to merge their newly implemented features to.
Pull requests should always be made to this branch (except for critical fixes), and could possibly break the code.
The develop branch is therefore unstable and not guaranteed to work on any system.

### master (only updated by group members)
The master branch get's updates from tested states of the develop branch.
Therefore, the master branch should contain functional but experimental code.

### release-* (the official releases)
The release branch is identified by it's major and minor version number e.g. `release-0.6`.
The official release tags are always made on a release branch.
Release branches will typically branch from or merge tested code from the master branch to freeze the code for release.
Only critical patches can be applied through pull requests directly on this branch, all non critical features should follow the standard path through develop -> master -> release-*

