// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers 
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>

typedef int64_t int64;
typedef uint64_t uint64;

unsigned int GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // Sumcoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // Sumcoin: intermediate uint256 can overflow by 1 bit
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    bool fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int static GetNextWorkRequired_V2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
 
    // current difficulty formula, ERC3 - DUAL_KGW3, written by Christian Knoepke - apfelbaum@email.de
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    bool kgwdebug=false;
    int64_t PastBlocksMass = 0;
    int64_t PastRateActualSeconds = 0;
    int64_t PastRateTargetSeconds = 0;
    double PastRateAdjustmentRatio = double(1);
    arith_uint256 PastDifficultyAverage;
    arith_uint256 PastDifficultyAveragePrev;
    double EventHorizonDeviation;
    double EventHorizonDeviationFast;
    double EventHorizonDeviationSlow;
	
    //DUAL_KGW3 SETUP
    static const int64_t Blocktime = 30;
    static const unsigned int timeDaySeconds = 86400;
    int64_t pastSecondsMin = timeDaySeconds * 0.025;
    int64_t pastSecondsMax = timeDaySeconds * 7;
    int64_t PastBlocksMin = pastSecondsMin / Blocktime;
    int64_t PastBlocksMax = pastSecondsMax / Blocktime;
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
	
    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || 
        (int64_t)BlockLastSolved->nHeight < PastBlocksMin) {
	if(kgwdebug){
    LogPrintf("RETURN::kgwdebug::BLOCK %d ((int64_t)BlockLastSolved->nHeight < PastBlocksMin) bnPowLimit.GetCompact() %I  %08x %s\n",pindexLast->nHeight+1, bnPowLimit.GetCompact(), bnPowLimit.GetCompact(), bnPowLimit.ToString().c_str()); 
			}
        return bnPowLimit.GetCompact(); 
    }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        PastBlocksMass++;
        PastDifficultyAverage.SetCompact(BlockReading->nBits);
        if (i > 1) {
            if(PastDifficultyAverage >= PastDifficultyAveragePrev)
                PastDifficultyAverage = ((PastDifficultyAverage - PastDifficultyAveragePrev) / i) + PastDifficultyAveragePrev;
            else
                PastDifficultyAverage = PastDifficultyAveragePrev - ((PastDifficultyAveragePrev - PastDifficultyAverage) / i);
        }
        PastDifficultyAveragePrev = PastDifficultyAverage;
        PastRateActualSeconds = BlockLastSolved->GetBlockTime() - BlockReading->GetBlockTime();
        PastRateTargetSeconds = Blocktime * PastBlocksMass;
        PastRateAdjustmentRatio = double(1);
        if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(72)), -1.228));  //28.2 and 144 possible
        EventHorizonDeviationFast = EventHorizonDeviation;
        EventHorizonDeviationSlow = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
                if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast))
                { assert(BlockReading); break; }
        }
        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }
	
    //KGW Original
    arith_uint256 kgw_dual1(PastDifficultyAverage);
    arith_uint256 kgw_dual2;
    kgw_dual2.SetCompact(pindexLast->nBits);
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
         kgw_dual1 *= PastRateActualSeconds;
         kgw_dual1 /= PastRateTargetSeconds;
    }
    int64_t nActualTime1 = pindexLast->GetBlockTime() - pindexLast->pprev->GetBlockTime();
    int64_t nActualTimespanshort = nActualTime1;	

    if(nActualTime1 < 0) { nActualTime1 = Blocktime; }

    if (nActualTime1 < Blocktime / 3)
        nActualTime1 = Blocktime / 3;
    if (nActualTime1 > Blocktime * 3)
        nActualTime1 = Blocktime * 3;
    kgw_dual2 *= nActualTime1;
    kgw_dual2 /= Blocktime;
	
    //Fusion from Retarget and Classic KGW3 (BitSend=)	
    arith_uint256 bnNew;
    bnNew = ((kgw_dual2 + kgw_dual1)/2);
	
    // DUAL KGW3 increased rapidly the Diff if Blocktime to last block under Blocktime/2 sec   15seconds.	
    if(kgwdebug){ LogPrintf("START::kgwdebug::BLOCK %d nActualTimespanshort = %d Blocktime/6 = %d \n" , pindexLast->nHeight+1, nActualTimespanshort, Blocktime/6 ); }
    if(nActualTimespanshort < Blocktime/2){  
        if(kgwdebug){ LogPrintf("ISSHORT::kgwdebug::BLOCK %d PREDIFF: bnNew.GetCompact() %I %08x %s bnNew first  \n", pindexLast->nHeight+1, bnNew.GetCompact(), bnNew.GetCompact(), bnNew.ToString().c_str() ); }
	    const int64_t nLongShortNew1 = 85;  
        const int64_t nLongShortNew2 = 100; 
        bnNew = bnNew * nLongShortNew1;	
        bnNew = bnNew / nLongShortNew2;	
	if(kgwdebug){ LogPrintf("ISSHORT::kgwdebug::BLOCK %d POSTDIFF: bnNew.GetCompact() %I %08x %s bnNew second \n" , pindexLast->nHeight+1 , bnNew.GetCompact(), bnNew.GetCompact(), bnNew.ToString().c_str() ); }
    }

    //BitBreak BitSend
    const int64_t nLongTimeLimit = 30 * 30; //sumcoin: 15 minutes
    if(kgwdebug){
	LogPrintf("kgwdebug::BLOCK %d (pblock->nTime - pindexLast->GetBlockTime()) = %d (max: %d) PREDIFF bnNew.GetCompact() %I %08x %s \n", pindexLast->nHeight+1,(pblock->nTime - pindexLast->GetBlockTime()), nLongTimeLimit, bnNew.GetCompact(), bnNew.GetCompact(), bnNew.ToString().c_str());
    }

    // Reduce difficulty if current block generation time has already exceeded maximum time limit.
	if ((pblock->nTime - pindexLast->GetBlockTime()) > nLongTimeLimit){
	bnNew = bnPowLimit*15;
       	if(kgwdebug){
           	LogPrintf("ISLONG::kgwdebug::BLOCK %d bnNew.GetCompact() %I Maximum block time hit - adjusted diff to %08x %s\n",pindexLast->nHeight+1, bnNew.GetCompact(), bnNew.GetCompact(), bnNew.ToString().c_str()); 
	           }
    }

    // Debug
    if (bnNew > bnPowLimit){
        //LogPrintf("BNNEWLARGE::BLOCK %d We wanted to set diff to %08x bnNew.GetCompact() %I but bnPowLimit is %08x  bnPowLimit.GetCompact() %I \n", pindexLast->nHeight+1, bnNew.GetCompact(), bnNew.GetCompact(), bnPowLimit.GetCompact(), bnPowLimit.GetCompact());
        bnNew = bnPowLimit;
    }
  
  if(kgwdebug){
           	LogPrintf("RETURN::kgwdebug::BLOCK %d bnNew.GetCompact() %I Maximum block time hit - adjusted diff to %08x %s\n",pindexLast->nHeight+1, bnNew.GetCompact(), bnNew.GetCompact(), bnNew.ToString().c_str()); 
			}
  
  

  return bnNew.GetCompact();



}


unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
   
   int DiffMode = params.DiffMode;

   if (pindexLast->nHeight+1 >= params.DiffModeV2) { DiffMode = 2; }

   if (DiffMode == 2) { return GetNextWorkRequired_V2(pindexLast, pblock, params); }
   if (DiffMode == 1) { return GetNextWorkRequired_V1(pindexLast, pblock, params); }
   
   return GetNextWorkRequired_V2(pindexLast, pblock, params); 
}

