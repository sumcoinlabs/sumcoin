// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "arith_uint256.h"
#include "bignum.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(9999) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    txNew.nTime = nTimeTx;

    CBlock genesis;
    genesis.nTime    = nTimeBlock;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis); //

    // CBigNum bnTarget;
    // bnTarget.SetCompact(genesis.nBits);

    // Generate a genesis block
    // Find one quicker if you start from a higher setting and launch a new process for each
    // (ie)
    // genesis.nNonce = 0;
    // make
    // ./sumcoind
    // genesis.nNonce = 1000000000;
    // make
    // ./sumcoind
    // .......

    // while (genesis.GetHash() > bnTarget.getuint256())
    // {
    //     if (genesis.nNonce % 1048576 == 0)
    //         printf("n=%dM hash=%s\n", genesis.nNonce / 1048576,
    //                 genesis.GetHash().ToString().c_str());
    //     genesis.nNonce++;
    // }
    // uint256 hash = genesis.GetHash();
    // printf("%s\n", hash.ToString().c_str());
    // printf("%s\n", genesis.ToString().c_str());
    // printf("%s\n", genesis.hashMerkleRoot.ToString().c_str());
    // genesis.print();

    // assert(false);

    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "We Forgot What Dr King Believed In NY Times";
    const CScript genesisOutputScript = CScript();
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 * 2022-02-01 16:27:26 CBlock(hash=000000d384b2c2ee13d10c0dc983052635e304ea8631136c81de0b76181891f5, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000,
 * hashMerkleRoot=d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4, nTime=1554579300, nBits=1e00ffff, nNonce=4097542, vtx=1, vchBlockSig=)
2022-02-01 16:27:26   Coinbase(hash=d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4, nTime=1554579000, ver=1, vin.size=1, vout.size=1, nLockTime=0)
    CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295),
    coinbase 04ffff001d020f27274669676874696e6720666c61726573206f6e206f7574736b69727473206f6620547269706f6c69)
    CTxOut(empty)
  vMerkleTree: d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("000000f40beaad5804ce621cca107c37dccd119e887625fe79fe0f4e161f6219");
        consensus.powLimit =            uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); //24
        consensus.bnInitialHashTarget = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); //32
        consensus.nTargetTimespan = 2 * 60 * 60;  // 2 hours
        consensus.nStakeTargetSpacing = 1 * 60; // 60 second block spacing
        consensus.nTargetSpacingWorkMax = 90 * consensus.nStakeTargetSpacing; // 90 mins
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24 * 1; // 1 day minimum age for coin age
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90; // 30 days      //60 * 60 * 24 * 90; // minimum age for coin age
        consensus.nModifierInterval = 6 * 60; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 100;
        // consensus.SegwitHeight = 1001;

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =   uint256S("0x00000000000000000000000000000000000000000000000000027c1fdfd3b71a"); // 30000

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid =  uint256S("0x8c7e4792f02e8affc807faec3b2db58ae618518336fa9e7c7cfb7acf59993b1b");  // 30000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xa2;
        vAlertPubKey = ParseHex("04c7f7f456da3181ee07967bce94b39b88fb313eedc3934579ef8365e3dec30618818af9331417af8d26d8a8f24bb1613a84516385d24846ec2a23479da4d9ea53");
        nDefaultPort = 3333;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1522621200, 1522621200, 211172335u, 0x1e00ffff, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000f40beaad5804ce621cca107c37dccd119e887625fe79fe0f4e161f6219"));
        assert(genesis.hashMerkleRoot == uint256S("0x2e655a309a2a8c29a2a13e1ea9f0706a3ac7ef20116b588aa316b4cb270a25d6"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("dnsseed.sumexplorer.com");
        vSeeds.emplace_back("dnsseed.suminsight.com");

       // vSeeds.emplace_back("dnsseed.sumcoin.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63);  // sumcoin: addresses begin with 'P' : Hex 3F
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,125); // hex 7D
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,187); // Hex bb
        base58Prefixes[EXT_PUBLIC_KEY] = {0xF5, 0x88, 0xB2, 0x1F};
        base58Prefixes[EXT_SECRET_KEY] = {0xF5, 0x88, 0xAD, 0xE5};

        // human readable prefix to bench32 address
        bech32_hrp = "sum";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0,         uint256S("0x000000f40beaad5804ce621cca107c37dccd119e887625fe79fe0f4e161f6219")},
                {1,         uint256S("0x000000002ad7a706fac8eedc222745b191ab97713027ef2d1fe80027fb8eb61f")},
                {10,        uint256S("0x0000000036d1d1cfdc4b65aad81e58c1ba2c12077d3f2e0d7356bb2006cc0c89")},
                {100,       uint256S("0x000000f3edbd544605bfa03dbcc2f3f1c6aa3aeebb47419630468192bd48c9fe")},
                {1000,      uint256S("0x00000077707fec1497a24ec59539f35606868deb92c1b22304155263c8c2a155")},
                {2000,      uint256S("0x000000439447fd424b6b699670086320474205bb17f68bfb1664bc4b3e731160")},
                {3000,      uint256S("0x00000014737d0e30699fd111a2342bc7eed832e6823a81ae85097c35cf315077")},
                {4099,      uint256S("0x000000123e76b6d922f35745f1c6ed7c94b893b5bd91809b15b3c964f94623fa")}, // Last PoW
                {4100,      uint256S("0xc3909a7db81f1f266a41f7682d11ab28cd0c9b13b02613f024dc6fe6e842fed2")},  // PoS
                {4101,      uint256S("0x22590dcc8bdc4c8dbdcc45e0e4490667837a88ae4819e05ea32c9f6db293267b")},
                {4110,      uint256S("0x4f510e77a0f55bf48772c6a1ec41596969ba4028e399f596c9a939e829097549")},
                {5000,      uint256S("0xb53a2cbae0b2da2f381cd4778fe04b044569b80fd956e09e106073ddfdb63f37")},
                {10000,      uint256S("0x38020087925415205bd182dbe9a8b1f1a0b62c01383cb6d6d747c2d4d17ed8b5")},
                {15000,      uint256S("0x4c0c9eb8b04cf2c50090ccfcc62d6509b82e54bbea913a4ca41611ea4322c550")},
                {20000,      uint256S("0x6fa9b4241404b896fa6d4bcbc4434c2f9acc5224b53ab548a4dbd521a30383e5")},
                {25000,      uint256S("0x9d733daebd0126d34dd8d347ed2036f044d83e408c6da01a81e9e69e5e36df78")}
                {25000,      uint256S("0x8c7e4792f02e8affc807faec3b2db58ae618518336fa9e7c7cfb7acf59993b1b")}

            }
        };

        chainTxData = ChainTxData{
            // Data as of block 8c7e4792f02e8affc807faec3b2db58ae618518336fa9e7c7cfb7acf59993b1b (height 30000).

            //1554579000
            1686999379, // * UNIX timestamp of last known number of transactions
            30000,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the ChainStateFlushed debug.log lines)
            0.1 // * estimated number of transactions per second after that timestamp
                        // 1992832/(1635782211-1345400356) = 0.006862798
        };
    }
};

/**
Testnet (v3)
2022-02-04 17:44:50 CBlock(hash=0000002e5eb15d16e78e1d75faca882e7eff7552cc762e1dac2f669d4b72338a, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000,
hashMerkleRoot=d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4, nTime=1554579200, nBits=1e00ffff, nNonce=22166755, vtx=1, vchBlockSig=)
2022-02-04 17:44:50   Coinbase(hash=d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4, nTime=1554579000, ver=1, vin.size=1, vout.size=1, nLockTime=0)
    CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295),
    coinbase 04ffff001d020f27274669676874696e6720666c61726573206f6e206f7574736b69727473206f6620547269706f6c69)
    CTxOut(empty)
  vMerkleTree: d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4
    */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        // consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0000001bd1c72d5de6244be7b0e27115f513ee37db56f44724761d99fd29dd5b");
        consensus.powLimit =            uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); //24
        consensus.bnInitialHashTarget = uint256S("0000000fffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); //28
        // consensus.powLimit =            uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 32;
        // consensus.bnInitialHashTarget = uint256S("0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 40;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;  // one week
        consensus.nStakeTargetSpacing = 1 * 60;  // 1-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24 * 1; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000001bd1c72d5de6244be7b0e27115f513ee37db56f44724761d99fd29dd5b"); //0

        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xa1;
        vAlertPubKey = ParseHex("04e3cb47cc1fcf4826fa60007ccde6de75eb3e49d452c24a0cfad958a8b215f62925a4d418ffc0ca2ddcaa1f150e49440084ad475e5d48f12b08ebda76f00d341b");
        nDefaultPort = 13333;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 2;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1522621200, 1522621200, 186231177u, 0x1e00ffff, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000001bd1c72d5de6244be7b0e27115f513ee37db56f44724761d99fd29dd5b"));
        assert(genesis.hashMerkleRoot == uint256S("0x2e655a309a2a8c29a2a13e1ea9f0706a3ac7ef20116b588aa316b4cb270a25d6"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.empty();
        // vSeeds.emplace_back("tseed.sumcoin.org");
        // vSeeds.emplace_back("tseed2.sumcoin.org");
        // vSeeds.emplace_back("tseed.sumcoin-library.org");
        // vSeeds.emplace_back("testseed.sumcoin.info");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,64);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,126);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,188);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xF4, 0x88, 0xB2, 0x1F};
        base58Prefixes[EXT_SECRET_KEY] = {0xF4, 0x88, 0xAD, 0xE5};

        // human readable prefix to bench32 address
        bech32_hrp = "tsum";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {     0, uint256S("0x0000001bd1c72d5de6244be7b0e27115f513ee37db56f44724761d99fd29dd5b")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 0x1b83b33894d51be0b8b323bfab093f638915236e0e40ba3b52bb33fdbc4053cd (height 442735)
            0, // * UNIX timestamp of last known number of transactions
            0,     // * total number of transactions between genesis and that timestamp

                        //   (the tx=... number in the SetBestChain debug.log lines)
            0
            // 0.003020718 // * estimated number of transactions per second after that timestamp
                        // 863997/(1632053274-1346029522) = 0.003020718

        };
    }
};

/**
 * Regression test
 *
2022-02-04 17:44:50 CBlock(hash=0000002e5eb15d16e78e1d75faca882e7eff7552cc762e1dac2f669d4b72338a, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000,
hashMerkleRoot=d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4, nTime=1554579200, nBits=1e00ffff, nNonce=22166755, vtx=1, vchBlockSig=)
2022-02-04 17:44:50   Coinbase(hash=d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4, nTime=1554579000, ver=1, vin.size=1, vout.size=1, nLockTime=0)
    CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295),
    coinbase 04ffff001d020f27274669676874696e6720666c61726573206f6e206f7574736b69727473206f6620547269706f6c69)
    CTxOut(empty)
  vMerkleTree: d87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4
    */


class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.powLimit =            uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); //24
        consensus.bnInitialHashTarget = uint256S("0000000fffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); //28

        consensus.nTargetTimespan = 7 * 24 * 60 * 60; // two weeks
        consensus.nStakeTargetSpacing = 1 * 60; // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;

        consensus.nStakeMinAge = 60 * 60 * 24 * 1; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000001bd1c72d5de6244be7b0e27115f513ee37db56f44724761d99fd29dd5b");

        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xa0;
        vAlertPubKey = ParseHex("04383862439513e940f6fcbf62d365c162a5256920c2c25b0b4266fdee4a443d71cfe224dbccff6fdb2ea57a37eb0cbec5637ebea06f63c70ca093672fbdc27643");
        nDefaultPort = 23333;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1522621200, 1522621200, 186231177u, 0x1e00ffff, 1, 0);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000001bd1c72d5de6244be7b0e27115f513ee37db56f44724761d99fd29dd5b"));
        assert(genesis.hashMerkleRoot == uint256S("0x2e655a309a2a8c29a2a13e1ea9f0706a3ac7ef20116b588aa316b4cb270a25d6"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        m_is_test_chain = true;
        m_is_mockable_chain = true;
        checkpointData = {
            {
                {0, uint256S("0x0000001bd1c72d5de6244be7b0e27115f513ee37db56f44724761d99fd29dd5b")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,64);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,126);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,188);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xF4, 0x88, 0xB2, 0x1F};
        base58Prefixes[EXT_SECRET_KEY] = {0xF4, 0x88, 0xAD, 0xE5};

        bech32_hrp = "sumr";
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        //fMineBlocksOnDemand = true;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (gArgs.IsArgSet("-segwitheight")) {
        int64_t height = gArgs.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }
 }

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
