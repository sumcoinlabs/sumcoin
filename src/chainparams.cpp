// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2016 Sumtoshi
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2016-2020 The Sumcoin Core developers


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
    const char* pszTimestamp = "Fighting flares on outskirts of Tripoli";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static CBlock CreateGenesisBlockTest(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
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
        consensus.nSubsidyHalvingInterval = 500000;
        consensus.BIP16Height = 1; // Hopefully also first block
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("3563d19f66aa85f33d1898f51bd4d38708e776cdae1a18e0cad8332d03b9f067"); // Should be first block
        consensus.BIP65Height = 1000; // Neither have happened yet
        consensus.BIP66Height = 1000; //
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing = 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 5; // Go segwit
        consensus.nMinerConfirmationWindow = 10; // SPEED
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1553988600; // August 4, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1555788600; // August 4, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1553988600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1555788600; // September 5, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000158472d379749c45"); // to block 1048320

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xc12e7cae889b0dd49f6d3d39fc40ff7ec5aadbfffb20d6ac4cb461c05f88e638"); //1048320

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xd0;
        nDefaultPort = 3333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1554579000, 133964, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x37d4696c5072cd012f3b7c651e5ce56a1383577e4edacc2d289ec9b25eebfd5e"));
        assert(genesis.hashMerkleRoot == uint256S("0xb82fb0f59328af96928f3a7648461f3db41fbfc2fef4e5ec6f7cf78ca067eacc"));


        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("dnsseed.sumcoinpool.org");
	vSeeds.emplace_back("dnsseed.sumwallet.org");
        vSeeds.emplace_back("dnsseed.sumcoinwallet.org");
	vSeeds.emplace_back("dnsseed.sumnode.io");
	vSeeds.emplace_back("dnsseed.sumcoinmining.org");
	vSeeds.emplace_back("sumdnsseed.moonypool.com");
	vSeeds.emplace_back("dnsseed.sumfaucet.co");


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63); //  hexid = "3F"
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);  //  
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,200); //
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,191);  //  hexid = "BF"
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB4, 0x1C};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAB, 0xE6};

        bech32_hrp = "sum";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                 {  0,    uint256S("0x37d4696c5072cd012f3b7c651e5ce56a1383577e4edacc2d289ec9b25eebfd5e")},
                { 2880,  uint256S("0x31dfe91b64cbbb167b4e4c644ad7b008bb4bb8ca4e42aea02f938f445dc37cff")},
                { 5760,  uint256S("0x87de6e561bb0ab66095b272450cabbb0b11fb272e5ae37123ba1464a5de74f5c")},
                { 8640,  uint256S("0x19c100388b8a8bc0230f43c4355733e2b3375e08f1667449678ec14f66174aae")},
                {11520,  uint256S("0x0fcc3eacab1e532325f1c5bde0372b78e999a504a8eaaf3d4628038de6735d30")},
                {14400,  uint256S("0x6b55672438b707c59c95e7681a9914c4d201d2cdf5f42d902d53ed23042e028a")},
                {28800,  uint256S("0x8c7fdd9dd713a964e09339593ca82a738135d6cd82421198311d7a3931742e04")},
                {57600,  uint256S("0xedbe7a9e2e20c8c12fea25dc9be9268bfa308145fa68d2d2ce54dc405cf12e76")},
                {115200, uint256S("0xe386f62c30a46f6b565ffc8a3fdf73e5e76e9f2f400d5cba1911e7732b52bd8d")},
                {172800, uint256S("0x341f603628dbe328512d9507066241d9fcc4c81ac91dd9bc87e2184ed5787b80")},
                {201600, uint256S("0x6bbde0a86b6e1efa71333bb3636f02931ebb872157b48c674efde25054a571b3")},
                {216000, uint256S("0x7f1d6d0c386cc6a6d5eb85877adb039d7747c6047d84e311b7213bf237c78b7c")},
                {230400, uint256S("0x8857a4bd70e6aee0eec8902cd88ed378c3d73d625a3920c67d393c83176d12af")},
                {259200, uint256S("0x886fbe4eebd29e5aba9c3cb1d2319aaba6053d6e6956d7af79f99da19641895e")},
                {285120, uint256S("0x1b33c03b4a819112be0828d0cedcb2cb01c490ce224b9197ebb622bb13e26680")},
                {313920, uint256S("0x137bfb551a6d4e9136d241e1a2391ac310acc0475b0afa2c51cabda16cc1e055")},
                {342720, uint256S("0x5f4d51dba279f2cb7ae0d7ec85a4f204d3577510fc01ba2c2cf2032315d8a296")},
                {371520, uint256S("0x47eb97737152c9e3382336f27f700d950a98c705224a768aba806c2296d89b79")},
                {400320, uint256S("0x70de8d2e57f15f5e0b2e9abf603de8c788b0c7a77fe56011c78f582bfb0f131d")},
                {429120, uint256S("0x8811d063cd09444e831270d6f7dc72ce348774b216790da34c8c8ba33412f563")},
                {443520, uint256S("0xb17262650aeaf2eb01cab804e17f736f0942ac7a4a3f71461d7adeea5e34e768")},
                {452160, uint256S("0x51c53d431e8ebf232842fe37c81bf2fb4d40b171065a23cea9824d818e643ba5")},
                {457920, uint256S("0x717050f4a4a96f2d7b4c13fe2db70dc2998c31fea01607d9529b8dfeb434bc56")},
		{472320, uint256S("0x3dccd8aaef590f102c7e3cd8200eb666a68d0356a24c87b51617314adb73e24c")},
		{501120, uint256S("0xb0f6796740602fa097685f4d572090e956b33154914bd20ef2bedbf01a706d31")},
		{527040, uint256S("0x1b3eb4800f56305d50c86c373131faafead605efeb6bf61c5df4044a1aec4d7f")},
		{529920, uint256S("0xbe82c887af163c8c3a117497cfa909113067e25a8258b6644684c28d28ff1fdd")},
		{544320, uint256S("0x3687c221709bd7c1c0848097df91cf6b1ff413e352e4f65e1ced6f4c94e07ec7")},
		{558720, uint256S("0x25ed1e5a99938e5fd2503c4be4a435d7d582387f0d07587f5fccf03f7244282a")},
		{573120, uint256S("0x7eefb09f6c5ad6874352b4c089d0dd8e9cf34b4d2537bcbef6bd0ea70a065998")},
		{587520, uint256S("0x5952411159da3f15c29795673730166b1adb6d66e4abe681c1b6ea994589cb6b")},
		{601920, uint256S("0x794df734a9ddcf3b7d460de7fde59e2ef83e9311e19800662344c46b2cec66e5")},
		{616320, uint256S("0x2df1470ec7f86b972ca0d2d64de1306813e1e882e2e9f5cb4906a175311ef168")},
		{630720, uint256S("0xa1b2fc648d66e8cd93869e99264ba892420370c98c2c0850e64d4a88415f92f1")},
		{645120, uint256S("0x4ce5317488df663f0430caee55c602d2a29170812690b818f91a9c64fb4bfaac")},
		{659520, uint256S("0x03c1bd4a9458439c9f258875273253ded816070e06a0f6331f61bf8b8f41fd74")},
		{673920, uint256S("0x13ce1beec7b38cf76d3df4b6979a30cf69b4b0b0e34d481e28d427c9b09b5375")},
		{688320, uint256S("0x4dd168a040c72b40d35bda63273dd2127f3e2ac84f8aadbed415fc19be3fbc85")},
		{702720, uint256S("0xb5ac308d7c89dafa60abbb95b715945dd9b9672176dd280939ce75804e266a20")},
		{717120, uint256S("0x42c18d5a929ea22e18843c8922e9bf0951728b831cee1ac9712b9abd1e04d785")},
		{731520, uint256S("0x5550f8dd5325e477140b7bce941c47d46c97f26344006c4ab36b6db63caab896")},
		{760320, uint256S("0x479416d69a39b40f82055a36ced8fde8bff7cc9488d88c4c51adb327bdfa2dbc")},
		{789120, uint256S("0xa5286d30fc85315092839b73630c2463c683e4b6d698140dda6938752a250025")},
		{817920, uint256S("0xef7aa22745d200dd6966995139e3ef35ebd97561f32555bfdeada1c2cdbce8be")},
		{846720, uint256S("0x16590b7ad48ea99a484e1c2fa686895623c4bb9ed14d832717351a588cad742b")},
		{875520, uint256S("0x1b15368afbefac00ea2833cd0dc486464f7a2bc45cf849863d8ecbb959c343f3")},
		{904320, uint256S("0x91a89c6516c90db046e458484c2e72b91fbad522b7abc727dd12d500645a5969")},
		{933120, uint256S("0x48365c2a8bed4d5052d49550378c681ea2570a95c76317a4d182090be7272127")},
		{961920, uint256S("0x5dd8e1fbbfb66b4919d880ca374de804ddb7673d0a5b9cb231e6b1470a73d7d0")},
		{990720, uint256S("0x7a14741c8b5c10eab3464cd23b65554f1bc5c940e2ccb69461802e61e4f73b96")},
	       {1019520, uint256S("0xab7a59f310e2e86d81af0ca1f608d24127d8af934825c34ba3897eab1ab398cf")},
	       {1048320, uint256S("0xc12e7cae889b0dd49f6d3d39fc40ff7ec5aadbfffb20d6ac4cb461c05f88e638")},

		    
		    //{1077120, uint256S("0x")},
		    //{1105920,
		    

            }
        };

        chainTxData = ChainTxData{
            // Data as of block 78177.
            1554581344, // * UNIX timestamp of last known number of transactions
            100,  // * total number of transactions between genesis and that timestamp
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
        consensus.BIP16Height = 1; // always enforce P2SH BIP16 on testnet
        consensus.BIP34Height = 1; // Prolly 1, hash needs to be adjusted
        consensus.BIP34Hash = uint256S("1713222425d0b5b402b81c5fd9928708ebd2233ccdcd8b08ec19da962890650e");
        consensus.BIP65Height = 90000; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 90000; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.14159 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 1.25 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 10857; // 75% for testchains
        consensus.nMinerConfirmationWindow = 14476; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1533355200; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1536120000; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1533355200; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1536120000; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000dfe5e6a49ef2dd8");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xb56850a0d4abd5ecdf9f8abcc243997f0663e65494341b36b623b2695b522697"); //190

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xd3;
        nDefaultPort = 13333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlockTest(1523718257, 2086000341, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x8f4af36aa0bdb9ae5a34d191bcbd80748569e4ef2e47587f0a3f5749dde17eea"));
        assert(genesis.hashMerkleRoot == uint256S("0xccd37098b85fc0f190dc74b18c0d6a42f52ac8833348d6ff3663489fc66e31e2"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // TODO add seeds


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,125);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,8);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,253);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x06, 0x32, 0x89, 0xCC};
        base58Prefixes[EXT_SECRET_KEY] = {0x01, 0x37, 0x80, 0x96};

        bech32_hrp = "tsum";

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

        genesis = CreateGenesisBlock(1554579800, 659710, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x19decb2815da5a7779c72af78fe6268c2a76ec94e940503a6c3ffafb282ef397"));
        assert(genesis.hashMerkleRoot == uint256S("0xb82fb0f59328af96928f3a7648461f3db41fbfc2fef4e5ec6f7cf78ca067eacc"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("19decb2815da5a7779c72af78fe6268c2a76ec94e940503a6c3ffafb282ef397")},
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

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
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
