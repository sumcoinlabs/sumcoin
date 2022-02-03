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
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    // CBigNum bnTarget;
    // bnTarget.SetCompact(genesis.nBits);

    // Generate a genesis block
    // Find one quicker if you start from a higher setting and launch a new process for each
    // (ie)
    // genesis.nNonce = 0;
    // make
    // ./peercoind
    // genesis.nNonce = 1000000000;
    // make
    // ./peercoind
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
    const char* pszTimestamp = "Fighting flares on outskirts of Tripoli";
    const CScript genesisOutputScript = CScript();
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.BIP34Height = 1001;
        consensus.powLimit =            uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 32;
        consensus.bnInitialHashTarget = uint256S("0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 40;
        consensus.nTargetTimespan = 7 * 24 * 60 * 60;  // one week
        consensus.nStakeTargetSpacing = 10 * 60; // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24 * 1; // minimum age for coin age
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90; // 30 days      //60 * 60 * 24 * 90; // minimum age for coin age
        consensus.nModifierInterval = 6 * 60; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 500;

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =   uint256S("0x00000000000000000000000000000000000000000000000000000000000003E8"); // 1016

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid =  uint256S("0x0000000000194d2547fe3780bbef28a0bf1063be1efdb9e300d26e91d0d8a210");  // 380000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xa2;
        vAlertPubKey = ParseHex("04a8a03f1089ad6e9c13b7edc2ece07e9537ad7031575ae19c5ef69d5310f3a9ec86873fc0bf10c7da6c379d33cde306c13f58bcbcd556bceb6b1b35090a80e058");
        nDefaultPort = 9999;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1554579000, 1554579300, 464441699, 0x1d00ffff, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000009e42d72910839035870b79578f9b53941d742fa0f93b55a1c1b83c56"));
        assert(genesis.hashMerkleRoot == uint256S("0xd87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.empty();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63);  // peercoin: addresses begin with 'P'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,125); // peercoin: addresses begin with 'p'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,187);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xF5, 0x88, 0xB2, 0x1F};
        base58Prefixes[EXT_SECRET_KEY] = {0xF5, 0x88, 0xAD, 0xE5};

        // human readable prefix to bench32 address
        bech32_hrp = "sc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0,         uint256S("0x000000009e42d72910839035870b79578f9b53941d742fa0f93b55a1c1b83c56")},
                {100,       uint256S("0x0000000000478c57c5b8d62ab73353f6f5c9d538808912e7111ce90f20460d6b")},
                {500,       uint256S("0x00000000001104d16645f5f56f352b92ba6addce22f410ee759403f7a5f4036e")},
                {1000,      uint256S("0x0000000000194d2547fe3780bbef28a0bf1063be1efdb9e300d26e91d0d8a210")} // Last PoW
                // {1005,      uint256S("0x14cc1d196eab184326176891c275cbe699185d91a52234c9422f42599e05b8bf")}, // PoS
                // {1016,      uint256S("0x91afaf0d08d45776e67fddeb82631acd1a351b760d5a1ee8f7b05493b1883b06")}
                // {1284,      uint256S("0xbfd2f5e71d080d021e34334827b27f4236bcfb4b71a18aee328a14165068d817")}
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 967c14abf21214639aeff0a270c4543cd3b80fe53178384ac5aa3c277662f1d0 (height 589659).
            1554580504, // * UNIX timestamp of last known number of transactions
            1000,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the ChainStateFlushed debug.log lines)
            0 // * estimated number of transactions per second after that timestamp
                        // 1992832/(1635782211-1345400356) = 0.006862798
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        // consensus.BIP16Height = 0;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00");
        consensus.powLimit =            uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 20;
        consensus.bnInitialHashTarget = uint256S("0x00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 32;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;  // one week
        consensus.nStakeTargetSpacing = 10 * 60;  // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 8; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000f3288c058d3fef6e6de1ff85e2d67c6a87b46d3f75a5289c6fde68b2"); //1135275

        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xa2;
        vAlertPubKey = ParseHex("04e7cf35e1cdf80e8930abd12a54bbfc7f11967357798197184d93b5aceff942221d22c1c885a556ff27a13c9172fdd22f4e392a6f3f02f04149142b0220b7191f");
        nDefaultPort = 19999;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 2;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1554579000, 1554579200, 864939, 0x1e0fffff, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000002cea8efe2ac2fb8092e60c40e43d96c196852e84794a2ec505612836f8f"));
        assert(genesis.hashMerkleRoot == uint256S("0xd87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.empty();
        // vSeeds.emplace_back("tseed.peercoin.net");
        // vSeeds.emplace_back("tseed2.peercoin.net");
        // vSeeds.emplace_back("tseed.peercoin-library.org");
        // vSeeds.emplace_back("testseed.ppcoin.info");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,64);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,126);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,188);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xF4, 0x88, 0xB2, 0x1F};
        base58Prefixes[EXT_SECRET_KEY] = {0xF4, 0x88, 0xAD, 0xE5};

        // human readable prefix to bench32 address
        bech32_hrp = "tpc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {     0, uint256S("0x000002cea8efe2ac2fb8092e60c40e43d96c196852e84794a2ec505612836f8f")},
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
 */

class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.powLimit =            uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 20;
        consensus.bnInitialHashTarget = uint256S("0x00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 32;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60; // two weeks
        consensus.nStakeTargetSpacing = 10 * 60; // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;

        consensus.nStakeMinAge = 60 * 60 * 8; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000002cea8efe2ac2fb8092e60c40e43d96c196852e84794a2ec505612836f8f");

        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xa1;
        vAlertPubKey = ParseHex("04383862439513e940f6fcbf62d365c162a5256920c2c25b0b4266fdee4a443d71cfe224dbccff6fdb2ea57a37eb0cbec5637ebea06f63c70ca093672fbdc27643");
        nDefaultPort = 19999;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1554579000, 1554579200, 864939, 0x1e0fffff, 1, 0);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000002cea8efe2ac2fb8092e60c40e43d96c196852e84794a2ec505612836f8f"));
        assert(genesis.hashMerkleRoot == uint256S("0xd87e2823fecc64d04b2475a48d8ae4dba9d0e46addaaaa9150b6a666e555f3b4"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        m_is_test_chain = true;
        m_is_mockable_chain = true;
        checkpointData = {
            {
                {0, uint256S("0x000002cea8efe2ac2fb8092e60c40e43d96c196852e84794a2ec505612836f8f")},
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

        bech32_hrp = "scr";
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
