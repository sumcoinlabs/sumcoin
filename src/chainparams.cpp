// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
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
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "USA Launches 3 missles in Syria, 2018";
    const CScript genesisOutputScript = CScript() << ParseHex("041880a07bac6e19ba6da66aec00cd7b824bccaeec805b84bd29941b13d31246dafb2d2914984b7080a34e8c9f51a6baf2c1fa3b48fc5eea80d8359ac9276dc17d") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1000000;
        consensus.BIP16Height = 1; // Hopefully also first block
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("1713222425d0b5b402b81c5fd9928708ebd2233ccdcd8b08ec19da962890650e"); // Should be first block
        consensus.BIP65Height = 90000; // Neither have happened yet
        consensus.BIP66Height = 90000; // 
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 3.14159 * 24 * 60 * 60; // pi days (why), or  ~271433 seconds
        consensus.nPowTargetSpacing = 1.25 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 10857; // 75% of 8064
        consensus.nMinerConfirmationWindow = 14476; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1533355200; // August 4, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1536120000; // August 4, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1533355200; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1536120000; // September 5, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000595a9ab76b42f"); // TODO

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xb56850a0d4abd5ecdf9f8abcc243997f0663e65494341b36b623b2695b522697");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfd; 
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xd3;
        nDefaultPort = 3333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1523718257, 2086000341, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x8f4af36aa0bdb9ae5a34d191bcbd80748569e4ef2e47587f0a3f5749dde17eea"));
        assert(genesis.hashMerkleRoot == uint256S("0xccd37098b85fc0f190dc74b18c0d6a42f52ac8833348d6ff3663489fc66e31e2"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("dnsseed.sumcoinpool.org");
        vSeeds.emplace_back("dnsseed.sumcoinwallet.org");
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63); //
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,200); //IDK
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,191);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB4, 0x1C};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAB, 0xE6};

        bech32_hrp = "sum";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = { 
            {
                {    0, uint256S("0x8f4af36aa0bdb9ae5a34d191bcbd80748569e4ef2e47587f0a3f5749dde17eea")},
				{ 1000, uint256S("0x9bc8eda5e597adb6c98cd39381682e44512b8d0fea81f10d6f7734aa5f73dcda")},
				{10000, uint256S("0x2b2c78658d5a9c8b14788cab7e37177dd92dd18462d1cac5827a7ce3b576a78d")},
				{20000, uint256S("0xd9b33b423ea9dec4959aa0b68fe67b6635c0c5a0b1ecc42af330c1c54d3082c6")},
				{30000, uint256S("0x51d795db5291cd668f744789297acb4edfd7180f00e8582b43749c57654e6c6e")},
				{40000, uint256S("0x2387bba8cf63242d95b2a0635dbdbc7f4e63c158cf28f3b89841f793f86564be")},
				{50000, uint256S("0xbd44b23287530556d148c44740fd0c15217231f3b76383e1d888de3dbe90b40a")},
				{60000, uint256S("0xb56850a0d4abd5ecdf9f8abcc243997f0663e65494341b36b623b2695b522697")},
				{70000, uint256S("0x66e3f8d48a6ad07435c8953344a235edd24dd6aa3372267f0b5c1a7396b3b37e")},
                                {78177, uint256S("0x075f8d0c7cadd3c479cf865ed87c644a4e89433686f8664c5f0081803620484b")},
		    		{90000, uint256S("0x4281442e2586fa16db1d50a51382da270b8821d34d7acb91bcabc21190f2fa8d")},
		               {100000, uint256S("0x20e6af5220f32554b7256a03027bbd13be28b38b2e9dd6e3c422ca977b35a8e2")},
		    	       {110000, uint256S("0xbded4d473ffa294ca5d5340ff868217fc45d7b3de54e7200a2c9a3e66ec704ed")},
		    	       {120000, uint256S("0xf1279ee8de606238a821dd91bd55cbcad30a98bd318ba8dfcdad9c22264c070d")},
		    	       {130000, uint256S("0x74993e44bd5cc38b5586fa8d198e4ced0b97025c173427bd9a836a3b0eddda23")},
		    	       {140000, uint256S("0x5fb47ccf5261043b7544066915b7f2698cb2e012ba2faae640b4f1f684448da3")},
		    	       {150000, uint256S("0xa701ea0eeff59372ae6165f081e25aab9fc0ef0ea6b1fa7a71a491f11ad01622")},
		    	       {160000, uint256S("0x6352dd343ba3904f20e9d19fcdbceffeb123f68d487b5d86eb0d17663e9f57d9")},
		    	       {170000, uint256S("0xc39c8ac3a3d4affe8cb98008f890b27e29ab0177be19f3129a4f497b3b6aa251")},
		               {180000, uint256S("0x697b6b2423ab8fd025d2901eabd1468e86549aefd3852a26e8ba55d357691add")},
		    	       {190000, uint256S("0x6e6aa26760605d1846a5ae1b6dc074d00cb71140d86cd097ee1ac481d1bae2c4")},
		    	       {200000, uint256S("0x0b083717b770a07975725ad1f00c8e18e00cd226227140bb7a7984638c435b1b")},
		    	       {210000, uint256S("0x9cb37786058462b692f96a0b1957a084c6f92b8d9fee4e9b672d5e721af8aea4")},
		   	       {220000, uint256S("0x2968582bb180670b1ecdb2c44f3cee13752f1ac1805d3ba522a39a3fe5865333")},
		    	       {230000, uint256S("0x90d256fdfa47423028e32a43d99b5d2b6f117d9c7796f2eb77570bf5cd38d4f6")},
		   	       {240000, uint256S("0xb88d24a0545eb9c885b6193360395a386a8bd79da04efb06fc63fce0d68db46b")},
		    	       {250000, uint256S("0x95913038a6c0ed045c84fa423285403cdd32e815c7fecffdf432c1f459df05bd")},
		    	       {260000, uint256S("0xb739e850c82a9dec8181648ad21576820c018c54b05c9a2de5f957c84b4d2fd2")},
		    	       {270000, uint256S("0xbd51309238e5fd26216ba00d3b7791e57921c4b2f158392c7b3c0e49c0d16cce")},
		    	       {280000, uint256S("0xebfd45fe1860690eeb178053dca1798c74667c0d1d0ac3474724929048b2afef")},
		    	       {290000, uint256S("0x73ca48e470af3ab951ea627b4524a6d9bd380f3280c5abc3840490401c25086c")},
		    {300000, uint256S("0x7454bb7e8d14b9267fe35465b18d9d05d268ae4a1bc40dceba5c16343e83b027")},
		    
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 78177.
            1532280528, // * UNIX timestamp of last known number of transactions
            82400,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.01     // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 1000000;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 1; // Prolly 1, hash needs to be adjusted
        consensus.BIP34Hash = uint256S("3ba02990b3c3598d04805b7a4d030b214be787ef5df5af7fcf4f1c05ee0879d5");
        consensus.BIP65Height = 300; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 300; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.14159 * 24 * 60 * 60; // pi
        consensus.nPowTargetSpacing = 1.25 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000000000000000ff");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x69231582d213d14e891cf6c863625dfcd567872fadf2c1f9ab7dca58f1b15d35"); //190

        pchMessageStart[0] = 0xf6;
        pchMessageStart[1] = 0xc7;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd1;
        nDefaultPort = 13333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1523718091, 387183082, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xe1309964e3ac20bd3bf8f7cdd9ccfc9b5a6a779b9975abc1c89c132db618048c"));
        assert(genesis.hashMerkleRoot == uint256S("0xccd37098b85fc0f190dc74b18c0d6a42f52ac8833348d6ff3663489fc66e31e2"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,125);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,8);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,253);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x06, 0x32, 0x89, 0xCC};
        base58Prefixes[EXT_SECRET_KEY] = {0x01, 0x37, 0x80, 0x96};

        bech32_hrp = "tsum";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("e1309964e3ac20bd3bf8f7cdd9ccfc9b5a6a779b9975abc1c89c132db618048c")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block a0afbded94d4be233e191525dc2d467af5c7eab3143c852c3cd549831022aad6 (height 343833)
            1523718091,
            0,
            0.01
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.14159 * 24 * 60 * 60; // pi
        consensus.nPowTargetSpacing = 1.25 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1532286178, 780485, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x05cb65a5a01fe1f3d898c86b76eb900fae6616a42fb93ce8eff0c989c69eeeeb"));
        assert(genesis.hashMerkleRoot == uint256S("0xccd37098b85fc0f190dc74b18c0d6a42f52ac8833348d6ff3663489fc66e31e2"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = {
            {
                {0, uint256S("05cb65a5a01fe1f3d898c86b76eb900fae6616a42fb93ce8eff0c989c69eeeeb")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,125);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,253);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x89, 0xCC};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x81, 0x96};

        bech32_hrp = "rsum";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
