# Sumcoin Engineering Handoff: September 25-26, 2026

> **Purpose:** High-detail technical handoff for a future developer, maintainer, or AI assistant. It records the Sumcoin debugging, fixes, experiments, merged pull requests, build/deployment work, test evidence, unresolved issues, and reasoning from September 25-26, 2026.
>
> **Repository:** `https://github.com/sumcoinlabs/sumcoin`
>
> **Primary branch:** `main`
>
> **Repository snapshot before this document:** `f1030cfcb16f38d6bf47d18d67c9a0541f3836a6`
>
> **Reading rule:** Distinguish **merged / implemented**, **observed / verified**, **strongly suspected**, and **deferred / unresolved**. Do not silently upgrade an observation into a proven network-wide conclusion.

---

## 1. Executive summary

During September 25-26, 2026, several long-standing Sumcoin issues were investigated and fixed.

1. **Disconnected coinstakes could leave original wallet inputs falsely marked spent until wallet restart.**
   - Fixed in PR #37 by abandoning disconnected wallet-owned coinstakes immediately.
   - A focused regression test was added.

2. **Coinstake debug logging could read `txNew.vout[2]` even when a third output did not exist.**
   - Fixed in PR #38.
   - Logging-only; no consensus change.

3. **Catch-up could stall because one peer held the exact next block needed while other peers delivered later blocks.**
   - Final solution merged through PR #39: after 15 seconds, fail over the blocking peer when the active chain is behind the best header tip and alternative validated download peers exist.

4. **PoS minting could begin before the active chain reached the best known header height.**
   - Fixed in PR #39.
   - Minter now requires both header catch-up and verification progress >= 0.996.

5. **Minting GUI refreshes were too frequent.**
   - The Minting table refreshed expensive age-related columns every 250 ms.
   - Changed to 60 seconds in PR #39.

6. **The largest staking problem was an inherited Peercoin-style lock architecture.**
   - `CreateCoinStake()` could hold `cs_main` and `cs_wallet` for long scans.
   - Large wallets could spend tens of seconds or minutes inside the search.
   - A first attempt released `cs_main` but still held `cs_wallet`; logs showed this was not enough.
   - Final PR #40 releases both long-lived locks for the expensive scan, briefly locks only specific shared-state operations, and validates chain tip and wallet inputs afterward.
   - `PoSMiner()` also stops treating a stale/rejected candidate like a successful mint.

7. **After PR #40, remaining 3-7 second macOS beachballs were sampled and traced primarily to the Qt transaction-model update path, not the staking scan.**
   - This remains a separate GUI task.

8. **After the merged staking fix was deployed to other significant staking nodes overnight, observed network behavior improved.**
   - No new visible `?` / “Generated but not accepted” stakes in the tested wallet the next morning.
   - Explorer difficulty rose from the earlier ~1.1-2 range to about 3.4.
   - Explorer estimated network hashrate recovered above 200M H/s.
   - This is consistent with the expected effect, but not controlled proof that the patch alone caused those network metrics.

---

## 2. Merged pull requests and commits

### PR #37 - `wallet: abandon disconnected coinstakes`

- PR: `https://github.com/sumcoinlabs/sumcoin/pull/37`
- Merge commit: `7b14fee09f5341d716b24dfe9c2c1f418f606ff5`
- Files:
  - `src/wallet/wallet.cpp`
  - `src/wallet/test/wallet_tests.cpp`

### PR #38 - `wallet: avoid out-of-bounds coinstake logging`

- PR: `https://github.com/sumcoinlabs/sumcoin/pull/38`
- Merge commit: `b7e9ad634854872ebf0541c785313b23bd297328`
- Feature commit: `78c2edef6b73be56e1bfc9e0d89379311dfd2e2f`
- File:
  - `src/wallet/wallet.cpp`

### PR #39 - catch-up failover, minter sync guard, Minting refresh reduction

- PR: `https://github.com/sumcoinlabs/sumcoin/pull/39`
- Merge commit: `807e1a20f21ee5496df2cdee0f150b81f580c288`
- Included commits:
  - `d76d328ecf83aa87942b5baafc6599bf9ac47189` - `net: keep catch-up failover active behind header tip`
  - `fc8bddfd2371103fc035f64310b212bd13656440` - `qt: reduce excessive minting table refreshes`
  - `70a733f30ad82f02c99af48cf10e312464b6dcd4` - `stake: wait for known header tip before minting`
- Files:
  - `src/net_processing.cpp`
  - `src/miner.cpp`
  - `src/qt/mintingtablemodel.cpp`

### PR #40 - `staking: avoid long chain and wallet locks during coinstake search`

- PR: `https://github.com/sumcoinlabs/sumcoin/pull/40`
- Feature commit: `688080a3a1c93950d9e42ec86acd2d5a360a4654`
- Merge commit: `f1b800b5209767eee81b99baad8e87a68457beab`
- Files:
  - `src/miner.cpp`
  - `src/wallet/wallet.cpp`
  - `src/wallet/wallet.h`

### README commits after PR #40

- `c460492c77702fb024892fbfcae2452eac80cadd`
- `f1030cfcb16f38d6bf47d18d67c9a0541f3836a6`

The README header now includes live badges for CI, latest release, total downloads, MIT license, open issues, and last commit.

At the time checked, historical GitHub release assets totaled about **1,131 downloads**. This is release-asset download count, not unique users.

---

## 3. September 25 catch-up investigation history

The IBD/catch-up work went through several experiments before the final retained implementation.

Relevant commits:

- `9d25d3bac77175f7b020aa903f868fcd0ba3e370` - fail over stalled IBD blocking blocks
- `91a941054035363519e1bb8c2a10ac46c6a4601f` - fail over stalled next IBD block
- `82b4795678244d7bfb3447ab5e5e274bc32a284f` - quarantine IBD blocking peers without disconnecting
- `8e42b4ab389b985448e6fda5737c1ec4d09f170f` - reassign only stalled next IBD block
- `98639f2245134fc63ed2286e14f4cf77a605c700` - restore proven blocking-peer failover
- `444482dc49e55407ef16bb1ba7ba0add71321b00` - directly hand off stalled IBD tip block
- `62833bc44778acfea5e3a69128005b0376225125` - time exact IBD tip blockage
- `4c211110238a4604dfd6d2863aff6df0879fe7ca` - keep proven IBD blocking-peer failover
- `d76d328ecf83aa87942b5baafc6599bf9ac47189` - final: keep catch-up failover active behind header tip

Future maintainers should treat the final merged behavior as authoritative, not assume every intermediate experiment should be reintroduced.

---

## 4. Head-of-line block download stall

### Observed behavior

The node could have:

- headers far ahead of the active chain,
- multiple peers actively transferring data,
- later blocks arriving,
- but the exact next sequential block assigned to one slow/stuck peer.

This produced a head-of-line stall even without CPU saturation.

### Why `IsInitialBlockDownload()` was not enough

Current Sumcoin contains:

```cpp
static const int64_t DEFAULT_MAX_TIP_AGE = 2000 * 24 * 60 * 60;
```

in `src/validation.h`.

That is roughly 2000 days.

Because IBD can become false long before the active chain reaches the best known header tip, the final catch-up logic does not rely only on the IBD latch.

### Final merged behavior

PR #39 adds:

```cpp
static constexpr int64_t CATCHUP_BLOCKING_BLOCK_TIMEOUT = 15 * 1000000;
```

The failover condition is effectively:

```cpp
const CBlockIndex* tip = ::ChainActive().Tip();

if (pindexBestHeader != nullptr &&
    queuedBlock.pindex != nullptr &&
    tip != nullptr &&
    tip->nHeight < pindexBestHeader->nHeight &&
    queuedBlock.pindex->pprev == tip &&
    nPeersWithValidatedDownloads > 1 &&
    nNow > state.nDownloadingSince + CATCHUP_BLOCKING_BLOCK_TIMEOUT) {

    LogPrintf("Peer=%d is blocking catch-up at height %d, disconnecting for failover\n",
              pto->GetId(), queuedBlock.pindex->nHeight);

    pto->fDisconnect = true;
    return true;
}
```

It only acts when:

- an actual best header exists,
- active chain is behind it,
- the queued block is exactly the next block after the active tip,
- more than one validated-download peer exists,
- the peer has held the needed block for more than 15 seconds.

Observed live log:

```text
Peer=0 is blocking catch-up at height 1733376, disconnecting for failover
```

Catch-up continued after the failover.

### Safe conclusion

The specific head-of-line stall was observed and the new code recovered from it.

Do **not** claim that full IBD has been benchmarked and proven globally faster in every environment.

---

## 5. Minter starting before real header catch-up

Previously PoSMiner relied primarily on:

```cpp
GuessVerificationProgress(...) < 0.996
```

That can report a high percentage even if the node knows about headers beyond the active tip.

PR #39 now requires:

```cpp
tip != nullptr &&
pindexBestHeader != nullptr &&
tip->nHeight >= pindexBestHeader->nHeight
```

and:

```cpp
verification_progress >= 0.996
```

before minting begins.

This prevents the minter from starting merely because the progress heuristic looks complete while the node still knows it is behind the header tip.

---

## 6. Orphaned coinstake wallet-state bug

### Symptom

A locally minted PoS block could be accepted and later orphaned.

The wallet-owned coinstake became unconfirmed, but a coinstake cannot simply re-enter the ordinary mempool.

The wallet could continue to see its original staking input as spent until restart.

This created the “phantom spent” behavior.

### PR #37 fix

`CWallet::blockDisconnected()` now:

1. processes the disconnected block normally,
2. collects wallet-owned coinstake hashes,
3. leaves the original chain/wallet lock scope,
4. calls `AbandonTransaction()` for each disconnected wallet-owned coinstake.

The rationale is that a disconnected coinstake cannot be mempool-reaccepted, so leaving it merely unconfirmed can leave the source stake input falsely consumed.

### Regression test

Added test:

```text
coinstake_disconnect_releases_staked_input
```

The test:

- creates a wallet-owned output,
- creates a synthetic coinstake spending it,
- verifies the source is spent before disconnect,
- simulates disconnect,
- verifies the coinstake becomes abandoned,
- verifies the original input is no longer spent.

This is a direct regression test for the wallet-state bug.

---

## 7. Out-of-bounds coinstake logging

Previous debug logging accessed:

```cpp
txNew.vout[2]
```

without proving a third output existed.

PR #38 now checks:

```cpp
if (txNew.vout.size() > 2)
```

before referencing the third output.

This changes logging only. It does not change staking rules, reward calculation, or consensus.

---

## 8. Minting table performance

During testing the wallet had approximately:

- 12,700+ Minting-table rows,
- ~59,987-59,992 wallet UTXOs.

`MintingTablePriv::refreshWallet()` walks wallet transactions and reconstructs rows.

`MintingTableModel::updateAge()` marks the entire Minting model changed for age / coin-day / mint-probability related columns.

With dynamic sorting, this can be expensive.

Previously:

```cpp
timer->start(MODEL_UPDATE_DELAY);
```

with `MODEL_UPDATE_DELAY = 250 ms`.

PR #39 changed the Minting-specific timer to:

```cpp
timer->start(60 * 1000);
```

This is GUI-only. It does not alter PoS kernel search timing, consensus, or staking frequency.

---

## 9. Main staking bug: long-held `cs_main` and `cs_wallet`

The old staking architecture followed a Peercoin-style design.

Historically:

- `PoSMiner()` could hold `cs_main` and wallet lock around block creation.
- `CreateNewBlock()` held chain/mempool locks.
- `CreateCoinStake()` held `cs_main` and `cs_wallet`.
- A very large wallet was then scanned for a valid kernel.

Observed searches included durations around:

- 18-21 seconds,
- 34.6 seconds,
- 71 seconds,
- 246 seconds,
- 387 seconds.

Sumcoin targets about 60-second block spacing, so a search taking tens or hundreds of seconds is operationally significant.

---

## 10. First staking-lock attempt and why it was rejected

The first version:

- used named locks,
- released `cs_main` during the long search,
- kept `cs_wallet` held,
- passed a fixed parent into `CreateCoinStake()`,
- added stale-tip checks,
- started honoring `ProcessBlockFound()` failure.

Early results were encouraging and roughly ten stakes were found.

However, later logs showed:

```text
~00:20:58 active height 1734417
~00:37:58 next UpdateTip activity
```

Then heights 4418 through 4431 were processed in a burst.

Similar smaller bursts appeared elsewhere.

This strongly suggested that holding `cs_wallet` for the whole long scan was still interfering with callbacks or chain/wallet processing.

Therefore the first design was not accepted as final.

**Do not revert to “release `cs_main` but hold `cs_wallet` for the whole kernel scan.”**

---

## 11. Final PR #40 locking design

### `CreateCoinStake()` gets the exact parent

Signature changed to:

```cpp
bool CreateCoinStake(
    const CWallet* pwallet,
    CBlockIndex* pindexPrev,
    unsigned int nBits,
    int64_t nSearchInterval,
    CMutableTransaction& txNew);
```

This keeps the candidate tied to the same parent used to calculate difficulty.

### Named locks

```cpp
WAIT_LOCK(cs_main, lockMain);
WAIT_LOCK(cs_wallet, lockWallet);
```

### Parent check before search

```cpp
if (::ChainActive().Tip() != pindexPrev)
    return false;
```

### Short Chain-interface lock for `AvailableCoins()`

The `chain().lock()` scope is kept around `AvailableCoins()` only and released before the kernel scan.

### Snapshot mutable wallet configuration

Example:

```cpp
const bool fSplitCoins = pwallet->m_split_coins;
```

### Release both long-lived locks for the expensive scan

```cpp
{
    REVERSE_LOCK(lockWallet);
    {
        REVERSE_LOCK(lockMain);
        {
            // long candidate scan
        }
    }
}
```

### Brief `cs_main` lock only around chain-sensitive kernel checking

```cpp
bool fKernelMatch = false;

{
    LOCK(cs_main);

    fKernelMatch = CheckStakeKernelHash(
        nBits,
        pindexPrev,
        header,
        postx.nTxOffset + CBlockHeader::NORMAL_SERIALIZE_SIZE,
        tx,
        prevoutStake,
        txNew.nTime - n,
        hashProofOfStake);
}
```

This was deliberate. Current v0.5 kernel logic mostly follows the supplied parent chain, but older compatibility/debug/index paths still touch global chain/index state.

### Brief wallet lock only for key lookup

```cpp
CKey key;

{
    LOCK(cs_wallet);
    if (!pwallet->GetLegacyScriptPubKeyMan()->GetKey(..., key)) {
        ...
    }
}
```

### Revalidate parent after scan

```cpp
if (::ChainActive().Tip() != pindexPrev) {
    LogPrintf("CreateCoinStake : tip changed during stake search, retrying\n");
    return false;
}
```

### Revalidate selected wallet inputs

```cpp
for (const CTxIn& txin : txNew.vin) {
    if (pwallet->IsSpent(txin.prevout.hash, txin.prevout.n)) {
        LogPrintf("CreateCoinStake : selected input changed during stake search, retrying\n");
        return false;
    }
}
```

### Reward state tied to original parent

Reward calculation uses:

```cpp
pindexPrev->nMoneySupply
```

rather than silently switching to a new active tip.

---

## 12. `CreateNewBlock()` changes

PR #40 changes:

```cpp
LOCK2(cs_main, m_mempool.cs);
```

to named locks:

```cpp
WAIT_LOCK(cs_main, lockMain);
WAIT_LOCK(m_mempool.cs, lockMempool);
```

Then it releases chain and mempool locks around the wallet kernel search:

```cpp
{
    REVERSE_LOCK(lockMempool);
    {
        REVERSE_LOCK(lockMain);
        fCoinStakeCreated = pwallet->CreateCoinStake(...);
    }
}
```

After locks return:

```cpp
if (::ChainActive().Tip() != pindexPrev) {
    LogPrintf("CreateNewBlock(): tip changed during coinstake search, retrying\n");
    return nullptr;
}
```

---

## 13. `PoSMiner()` changes

The old outer:

```cpp
LOCK2(cs_main, pwallet->cs_wallet);
```

around block-template creation was removed.

Before signing, PoSMiner now revalidates the exact parent:

```cpp
LOCK2(cs_main, pwallet->cs_wallet);

if (::ChainActive().Tip() == nullptr ||
    pblock->hashPrevBlock != ::ChainActive().Tip()->GetBlockHash()) {
    LogPrintf("PoSMiner(): discarding stale stake candidate before signing\n");
    continue;
}
```

It then signs.

`ProcessBlockFound()` is now honored:

```cpp
if (!ProcessBlockFound(pblock, Params()))
    continue;
```

Only a successfully accepted block gets:

```cpp
reservedest.KeepDestination();
```

and the normal post-mint rest.

---

## 14. Safety reasoning for PR #40

- `CBlockIndex* pindexPrev` remains a stable block-index object even if it leaves the active chain; active-tip equality is rechecked.
- Staking candidates are copied into local snapshot-like structures for the unlocked scan.
- Wallet mutation is allowed during scan, so selected inputs are rechecked afterward.
- Chain movement is allowed during scan, so the parent is rechecked in multiple layers.
- `CheckStakeKernelHash()` is not assumed to be universally lock-free; it gets a short `cs_main` lock.
- Key lookup gets a short `cs_wallet` lock.
- Nested `REVERSE_LOCK` scopes restore the normal lock order when leaving the scan.
- The patch does not change kernel hash rules, stake age, difficulty formula, or block reward.

---

## 15. Ten-minute final-build live test

Local stakes extracted from the log:

```text
2026-09-26T01:30:18Z  014ac9d80b53  height=1734487  confirmations=17
2026-09-26T01:32:16Z  a466a2479c67  height=1734490  confirmations=14
2026-09-26T01:35:19Z  28057a7808db  height=1734494  confirmations=10
2026-09-26T01:36:41Z  c3859c8694d9  height=1734495  confirmations=-1
2026-09-26T01:38:25Z  e74e5335ba16  height=1734496  confirmations=-1
2026-09-26T01:40:23Z  fa6698a6f6ed  height=1734498  confirmations=6
2026-09-26T01:41:49Z  0ff2e6430fb5  height=1734499  confirmations=5
2026-09-26T01:43:07Z  cc8aa59c8803  height=1734501  confirmations=3
```

Two blocks later had negative confirmations, but log timing showed real branch competition rather than the old local lock blindness.

---

## 16. Genuine-fork example around `e74e5335...`

At `01:38:25Z`, local stake:

```text
e74e5335ba16a7f74fd7324a3a07027e5d378c3c6802f550e8eca544ab6bad73
```

was accepted at height 1734496.

The preceding timing line was roughly:

```text
CreateNewBlock() packages: 34607.84ms
```

At `01:38:32-01:38:33`, competitor compact-block data arrived and the competing branch was processed immediately.

At `01:38:52`, the local branch temporarily became active again.

At `01:40:00`, another competing branch won.

At `01:40:11`, stale-search protection logged:

```text
CreateCoinStake : tip changed during stake search, retrying
CreateNewBlock(): tip changed during coinstake search, retrying
```

This is key evidence that the node was processing network blocks during a long staking operation.

A `?` / “Generated but not accepted” transaction can therefore be a legitimate live-network fork outcome. PR #40 reduces avoidable stale-parent attempts; it cannot eliminate all normal PoS forks.


---

## 17. One-hour final-build result

The exact final PR #40 build was left running for about one hour before commit/push.

Observed:

- staking continued normally,
- stale-search retries appeared as designed,
- the earlier multi-minute “no tips, then many tips in a burst” pattern did not recur,
- GUI beachballs were much less frequent,
- remaining GUI freezes were roughly 3, 5, or 7 seconds.

This was considered sufficient to commit and push:

```text
688080a3a1c93950d9e42ec86acd2d5a360a4654
staking: avoid long locks during coinstake search
```

PR #40 was then merged into `main`.

---

## 18. Overnight network observation after deploying PR #40 broadly

After merge, the same code was deployed to the other significant staking nodes.

By the next morning:

- no new visible `?` stakes were present in the tested wallet screenshot,
- mint transactions continued appearing regularly,
- explorer PoS difficulty was roughly 3.4 near the right edge,
- earlier during debugging difficulty had fallen to roughly 1.1-2,
- explorer estimated network hashrate was back above 200M H/s and near roughly 230M at the right edge,
- earlier it had dropped to roughly 75-100M.

### Interpretation

This is consistent with the theory that lock-heavy staking was reducing effective network participation.

A node that spends long periods searching while blocking itself from timely chain/wallet updates can be technically “staking” but contributing less effectively than expected.

If several large staking nodes behave this way, fixing them can plausibly:

- reduce avoidable stale-parent searches,
- reduce unnecessary short-fork pressure,
- reduce long block gaps,
- improve effective stake participation,
- indirectly push difficulty upward as block spacing normalizes.

### Limitation

Do not state that PR #40 alone mathematically caused the difficulty increase.

The network is live and many variables affect block spacing.

Correct wording:

> The overnight improvement is consistent with the expected effect of deploying PR #40 to the major stakers, but it is not a controlled causal experiment.

---

## 19. Remaining macOS GUI beachball was captured

After PR #40, GUI responsiveness improved substantially, but occasional 3-7 second beachballs remained.

A macOS `sample` was captured during a real freeze.

The main GUI thread spent the majority of sampled time in a path involving:

```text
WalletModel::pollBalanceChanged()
TransactionTableModel::updateConfirmations()
Qt dataChanged()
QSortFilterProxyModel
TransactionTablePriv::index()
wallet transaction status lookup / tryGetTxStatus()
```

The staking thread existed in the sample, but the UI freeze was dominated by the transaction-model / proxy-model work.

### Relevant code behavior

`WalletModel::pollBalanceChanged()` uses:

```cpp
m_wallet->tryGetBalances(...)
```

and when block height changes it calls:

```cpp
transactionTableModel->updateConfirmations();
```

`TransactionTableModel::updateConfirmations()` emits:

```cpp
Q_EMIT dataChanged(index(0, Status), index(priv->size()-1, Status));
Q_EMIT dataChanged(index(0, ToAddress), index(priv->size()-1, ToAddress));
```

This marks the relevant fields changed across the full transaction table.

The GUI uses dynamic sort/filter proxy models.

On a large wallet, a block arrival can therefore trigger expensive remapping/status requests across a large model.

### Status

**Not fixed yet.**

This should be a separate GUI PR.

Do not mix it into staking/consensus code.

---

## 20. Peercoin comparison

Current Peercoin source was inspected to determine whether the staking lock issue had already been solved upstream.

At inspected Peercoin revision:

```text
1af7509666a7ce68b2c6102c772ac803d214fda1
```

Peercoin still broadly uses a lock-heavy staking design:

- `BlockAssembler::CreateNewBlock()` takes `LOCK(::cs_main)`.
- `CWallet::CreateCoinStake()` takes `LOCK2(cs_main, pwallet->cs_wallet)`.

Peercoin also has mitigating differences:

- its stake path caches tx/header data via `g_txindex->cachedTxs`,
- its block cadence is much slower than Sumcoin's roughly 60-second target,
- its staking implementation has continued evolving.

### Safe comparison statement

> Peercoin still uses the older model of holding chain/wallet locks through much of coinstake creation. Sumcoin PR #40 now releases the long-lived chain and wallet locks during the expensive candidate scan, briefly reacquires only the shared-state lock needed for a specific operation, then revalidates parent and selected inputs before using the result.

Do not claim Peercoin previously fixed this exact problem and Sumcoin merely failed to copy a patch.

---

## 21. Difficulty and effective staking participation

Sumcoin PoS difficulty responds to actual block timing.

Broad reasoning:

- slower accepted-block production tends to reduce difficulty,
- faster/more-normal accepted-block production tends to increase difficulty,
- stale or lock-blocked staking can reduce effective participation even if nominal wallet stake remains online.

Therefore PR #40 can influence difficulty **indirectly** by improving effective participation.

It does not directly set difficulty.

---

## 22. Unresolved consensus issue: commented `bad-diffbits` rejection

Current `src/validation.cpp` contains logic equivalent to:

```cpp
if (block.nBits != GetNextTargetRequired(
        pindexPrev,
        block.nFlags & CBlockIndex::BLOCK_PROOF_OF_STAKE,
        consensusParams)) {

    LogPrintf(...);

    // return state.Invalid(
    //     BlockValidationResult::BLOCK_INVALID_HEADER,
    //     "bad-diffbits",
    //     "incorrect proof of work/stake");
}
```

The invalid return is commented out.

### Status

**Unresolved and consensus-sensitive.**

### Critical rule

Do **not** simply uncomment this on mainnet.

If existing history or active peers have relied on permissive behavior, making the check mandatory could split the chain.

Any future fix needs:

- historical-chain audit,
- regtest/testnet validation,
- activation planning,
- release coordination,
- likely a height/time based activation mechanism.

---

## 23. Unresolved current-state issue: 2000-day `DEFAULT_MAX_TIP_AGE`

Current `src/validation.h` contains:

```cpp
static const int64_t DEFAULT_MAX_TIP_AGE = 2000 * 24 * 60 * 60;
```

This is approximately 2000 days.

Potential consequences:

- `IsInitialBlockDownload()` can stop meaning “actually caught up,”
- logic that gates on IBD state may disable too early,
- GUI/App Nap/current-state behavior may become misleading.

PR #39 intentionally avoided relying only on the IBD latch for catch-up failover and minter startup.

### Status

**Deferred.**

Audit all IBD consumers before changing this value.

---

## 24. Deferred GUI header-sync display issue

A separate GUI sync-display issue was identified.

The header-sync estimate uses elapsed time and a height-rate assumption. After enough time without new headers, the GUI can show misleading “Syncing Headers / Unknown / 100%” style states.

This was not fixed during these PRs.

### Status

**Deferred.**

---

## 25. PoS block reward concern

Observed staking rewards were around:

```text
5.999240 SUM
```

per minted block after relevant fees/cost effects.

It was explicitly noted that approximately 6 SUM per block appears too high and should be revisited.

### Status

**Separate economic/consensus work only.**

Do not change reward while working on performance, GUI, or networking.

A reward change needs:

- target issuance/inflation model,
- activation design,
- backwards-compatibility analysis,
- exchange/node coordination,
- testing.

---

## 26. macOS build environment

Primary development environment:

- Apple Silicon Mac
- macOS 15.6.1 during the work
- Xcode clang 17
- Homebrew `/opt/homebrew`
- Rosetta installed
- depends target `x86_64-apple-darwin16`

Known-good configure command:

```bash
./configure \
  --prefix="$PWD/depends/x86_64-apple-darwin16" \
  --enable-tests \
  --disable-gui-tests \
  --disable-bench \
  --with-gui=qt5 \
  CXXFLAGS="-pipe -O2 -Wno-enum-constexpr-conversion" \
  --disable-shared \
  --with-pic \
  --enable-benchmark=no \
  --with-bignum=no \
  --enable-module-recovery \
  --disable-jni
```

Build:

```bash
/usr/bin/arch -x86_64 /bin/zsh -c 'make -j4'
```

Build distributable DMG:

```bash
/usr/bin/arch -x86_64 /bin/zsh -c 'make -j4 && make deploy'
```

Expected outputs:

```text
Sumcoin-Qt.app
Sumcoin.dmg
```

Nonfatal warnings seen on macOS included duplicate libraries such as:

```text
-levent
-lpthread
-lQt5Core
-lQt5Gui
```

---

## 27. Current macOS release asset

The v0.20.0 GitHub release includes:

```text
Mac-Sumcoin-0.20.0.dmg
```

At inspection time:

- size approximately 11.4 MB
- GitHub SHA-256:
  `4d86a407ca344e56072287dcd29d2e13306d188413b1efe01ad703290e4acfd9`

Release page:

```text
https://github.com/sumcoinlabs/sumcoin/releases/tag/v0.20.0
```

---

## 28. Linux server build: Automake mismatch

On Ubuntu 22.04, a build failed:

```text
configure.ac:44: error: version mismatch.
This is Automake 1.16.5,
but the definition used by this AM_INIT_AUTOMAKE
comes from Automake 1.16.1.
```

Correct response is to regenerate the build system:

```bash
cd ~/sumcoin

./autogen.sh

./configure \
  --disable-tests \
  --disable-bench \
  --without-gui \
  --with-incompatible-bdb \
  --disable-shared \
  --with-pic \
  --enable-benchmark=no \
  --with-bignum=no \
  --enable-module-recovery \
  --disable-jni

make -j"$(nproc)"
```

This is a generated-build-system mismatch, not a Sumcoin source bug.

---

## 29. Linux build: Berkeley DB mismatch

The same server had Berkeley DB other than 4.8.

Configure initially stopped with:

```text
Found Berkeley DB other than 4.8, required for portable wallets
(--with-incompatible-bdb to ignore or --disable-wallet to disable wallet functionality)
```

Using:

```bash
--with-incompatible-bdb
```

allowed configure to continue, with the warning that wallets opened by this build are not portable in the old BDB-4.8 sense.

Use this knowingly; do not silently add it to release builds.

---

## 30. Linux build: stale `/usr/local` Boost conflict

Later compile failure:

```text
/usr/local/include/boost/thread/pthread/thread_data.hpp:60:5:
error: missing binary operator before token "("

#if PTHREAD_STACK_MIN > 0
```

Investigation found two Boost versions:

```text
/usr/local/include/boost/version.hpp -> 1_71
/usr/include/boost/version.hpp       -> 1_74
```

Libraries included:

```text
/lib/x86_64-linux-gnu/libboost_thread.so.1.74.0
/lib/x86_64-linux-gnu/libboost_system.so.1.74.0
/lib/x86_64-linux-gnu/libboost_filesystem.so.1.74.0
/usr/local/lib/libboost_filesystem.so.1.71.0
```

The compiler was using stale 1.71 headers in `/usr/local/include`.

Safe remediation:

```bash
mv /usr/local/include/boost /usr/local/include/boost-1.71-disabled
```

Then configure explicitly against Ubuntu Boost:

```bash
./configure \
  --disable-tests \
  --disable-bench \
  --without-gui \
  --with-incompatible-bdb \
  --with-boost=/usr \
  --with-boost-libdir=/usr/lib/x86_64-linux-gnu \
  --disable-shared \
  --with-pic \
  --enable-benchmark=no \
  --with-bignum=no \
  --enable-module-recovery \
  --disable-jni
```

Then:

```bash
make -j"$(nproc)"
```

After build, verify actual linkage:

```bash
ldd ~/sumcoin/src/sumcoind | grep boost
```

Desired result is system Boost 1.74, not stale 1.71 libraries from `/usr/local`.

---

## 31. Marketplace-server operational warning

One Ubuntu server reported:

```text
/ usage: 90.7% of 24.05GB
memory usage: 63%
swap usage: 100%
```

This was not the cause of the Boost compile error, but it is operationally important.

Before large builds/reindexing:

```bash
df -h
free -h
du -xh /root /var 2>/dev/null | sort -h | tail -50
```

Do not blindly delete blocks, chainstate, wallets, or build data.


---

## 32. CI and build context

Supporting work before and around this debugging period repaired build/CI behavior.

Relevant known context:

- core binary CI has passed on:
  - Linux x86_64
  - Linux ARM
  - macOS
  - Windows
- macOS Rosetta build fixes were merged earlier in PR #36:
  `https://github.com/sumcoinlabs/sumcoin/pull/36`

For deployment-only server builds, the preferred flags in these sessions were:

```bash
--disable-tests
--disable-bench
--without-gui
```

That is a deployment optimization, not permission to skip tests for code changes before merge.

---

## 33. Debug logging used for staking tests

Runtime categories were enabled with:

```bash
cd ~/sumcoin
./src/sumcoin-cli logging '["bench","net","cmpctblock"]' '[]'
```

This does not change staking rules.

It only enables extra logging.

Useful categories:

- `bench`
  - block-template timing
- `net`
  - network message activity
- `cmpctblock`
  - compact-block handling

This combination was useful for proving whether incoming blocks were being processed during a long stake search.

---

## 34. Useful log patterns for future investigations

Watch for:

```text
CreateNewBlock() packages:
CreateCoinStake : tip changed during stake search, retrying
CreateCoinStake : selected input changed during stake search, retrying
CreateNewBlock(): tip changed during coinstake search, retrying
PoSMiner(): discarding stale stake candidate before signing
CPUMiner : proof-of-stake block found
UpdateTip: new best=
generated block is stale
ProcessNewBlock, block not accepted
received: cmpctblock
received: blocktxn
received: block
```

Do not judge staking health from GUI icons alone.

Correlate:

- local block-found time,
- block hash,
- active-chain confirmation state,
- network arrival times,
- `UpdateTip` sequence,
- stale-search retry logs.

---

## 35. Meaning of `?` / “Generated but not accepted”

A `?` does not automatically mean the local minter malfunctioned.

A local PoS block can:

1. become active,
2. lose a branch race,
3. become orphaned,
4. become active again after another reorg,
5. lose again later.

This happened during the `e74e...` branch fight.

### Distinguish two categories

#### Avoidable local stale staking

Examples:

- node could not process a new tip because a long lock was held,
- search continued against obsolete local state,
- block was constructed from a parent the node should already have abandoned.

#### Legitimate live-network fork

Examples:

- local block was active first,
- competing valid branch arrived later,
- normal chain-trust selection changed the active branch.

PR #40 targets the first category.

It cannot and should not eliminate the second.

---

## 36. README state and cleanup still needed

README header was improved with live badges.

However, contradictions remain.

Examples identified:

- duplicate `### What is Sumcoin?` heading near the top,
- legacy text says `scrypt proof-of-work`,
- a specifications section elsewhere says `SHA-256`,
- the current network is proof-of-stake after v0.20.

### Status

Documentation cleanup still needed.

Verify protocol wording against actual source/current network before rewriting it.

---

## 37. Branch cleanup context

Branches previously verified as fully contained and safe to remove included:

```text
investigate/ibd-sync-performance
macos-rosetta-build-fixes
ci/update-github-actions
```

The PR #39 branch:

```text
investigate/minting-gui-performance
```

appeared to auto-delete after merge.

Historical branches such as:

```text
index
insight
rebase
reduce_txout
```

were intentionally left alone even if they showed zero commits ahead of `main`.

Do not delete historical branches solely because they are zero-ahead.

---

## 38. What is proven vs what is not

### Directly observed / verified

- disconnected coinstake release has a regression test,
- catch-up blocking-peer failover triggered in live logs,
- minter checks header catch-up and verification progress,
- Minting-table refresh changed 250 ms -> 60 s,
- final staking patch built successfully on macOS,
- stale-search retry logic triggered in live logs,
- network blocks were processed during a ~34.6 second staking operation,
- `e74e...` was a real branch fight,
- the earlier multi-minute burst pattern was not seen in the final short test,
- remaining short beachball sample was dominated by transaction-model/proxy-model work,
- the next-morning wallet screenshot showed no new visible `?` stakes in the observed overnight period.

### Strongly suggested but not scientifically isolated

- PR #40 reduces avoidable stale-parent minting,
- broad deployment improved effective network staking participation,
- observed difficulty/hashrate recovery is consistent with the patch,
- holding `cs_wallet` was a major contributor to the first-version stall.

### Not proven

- full IBD is always faster after PR #39,
- PR #40 alone caused the difficulty increase,
- all future orphaned stakes are eliminated,
- every future GUI beachball has exactly the same cause,
- block reward should be changed to a particular number without economic analysis.

---

## 39. Rules for future AI assistants or maintainers

1. Read PR #37, #39, and #40 before changing staking behavior.
2. Do not reintroduce a wallet lock around the full kernel scan.
3. Do not reintroduce a chain lock around the full kernel scan.
4. Keep parent identity tied to the `nBits` and candidate being searched.
5. If long work occurs outside wallet locks, revalidate selected wallet inputs afterward.
6. If long work occurs outside chain locks, revalidate the parent afterward.
7. Do not assume `!IsInitialBlockDownload()` means active chain reached best header.
8. Do not conflate the Minting-table 60-second GUI timer with staking behavior.
9. Do not classify every `?` stake as a bug without examining reorg logs.
10. Do not uncomment `bad-diffbits` rejection casually.
11. Do not change the ~6 SUM reward inside unrelated performance work.
12. Keep transaction-model GUI optimization in a separate PR.
13. Distinguish live improvement from controlled benchmarks.
14. Keep using the proven macOS Rosetta x86_64 workflow until native ARM is separately validated.
15. On Ubuntu, check for stale `/usr/local` dependencies before blaming source code.
16. Prefer one isolated change at a time, then build/test before commit/push.
17. Avoid force-closing the wallet during diagnostic runs because unclean shutdown can complicate interpretation of chainstate/replay behavior.
18. When testing staking, preserve exact timestamps and block hashes; “it looked better” is less useful than correlated logs.
19. Genuine shallow PoS forks are expected to remain possible.
20. Keep consensus/economic changes separate from performance fixes.

---

## 40. Recommended next work

### A. Fix the Qt transaction-list beachball

Reproduce with the large wallet.

Focus on:

```text
WalletModel::pollBalanceChanged()
TransactionTableModel::updateConfirmations()
QSortFilterProxyModel
TransactionTablePriv::index()
wallet transaction status lookup
```

Goal:

- preserve correct confirmation/status updates,
- avoid unnecessary whole-model proxy remapping/status work every block.

Do not blindly remove `updateConfirmations()`.

### B. Monitor PR #40 across major stakers

Track:

- accepted local stakes,
- orphan/`?` rate,
- stale-search retries,
- block spacing,
- difficulty,
- effective network participation,
- any return of multi-minute `UpdateTip` stalls.

### C. Audit IBD/current-state semantics

Review every use of:

```cpp
DEFAULT_MAX_TIP_AGE
IsInitialBlockDownload()
```

### D. Audit `bad-diffbits`

Treat as a possible consensus upgrade.

### E. Analyze PoS reward separately

Evaluate:

- issuance,
- current supply,
- active stake,
- target inflation,
- long-term security budget.

### F. Clean README contradictions

Fix duplicate headings and stale PoW/scrypt/SHA-256 text after verifying exact current protocol wording.

---

## 41. PR #40 merge-description reference

The merge rationale was:

> This fixes a staking performance and stale-tip issue inherited from the older Peercoin-style coinstake search.
>
> Previously, CreateCoinStake could hold cs_main and cs_wallet for the duration of a long stake search. On wallets with many staking outputs, that search can take tens of seconds or longer. During that time, incoming blocks and wallet updates could be delayed, causing the node to fall behind the active chain and potentially continue searching against an outdated tip.
>
> The change:
>
> - snapshots required staking state before the long search
> - releases cs_main and cs_wallet while scanning staking candidates
> - briefly reacquires cs_main only when chain/index state is needed
> - briefly reacquires cs_wallet only when wallet/key state is needed
> - verifies the active chain tip is still the expected parent after the search
> - verifies selected wallet inputs are still unspent before using the result
> - makes PoSMiner retry if the tip changed instead of continuing with a stale candidate
> - only keeps the reserved destination after ProcessBlockFound succeeds
>
> Local testing on a large staking wallet showed the node continuing to process incoming blocks during long stake searches instead of accumulating them and processing them in bursts afterward. Stale searches were detected and retried correctly, and staking continued normally for over an hour.
>
> This does not eliminate legitimate PoS chain forks or orphaned stakes caused by competing blocks. It prevents the local staking search from unnecessarily blocking chain processing and creating avoidable stale-parent attempts.

---

## 42. Quick reference: important code paths

### Networking / catch-up

```text
src/net_processing.cpp
PeerLogicValidation::SendMessages()
CATCHUP_BLOCKING_BLOCK_TIMEOUT
pindexBestHeader
state.vBlocksInFlight
```

### PoS block creation

```text
src/miner.cpp
BlockAssembler::CreateNewBlock()
PoSMiner()
ProcessBlockFound()
```

### Wallet staking

```text
src/wallet/wallet.cpp
CWallet::CreateCoinStake()
CWallet::blockDisconnected()
CWallet::AbandonTransaction()
```

### Wallet declaration

```text
src/wallet/wallet.h
CWallet::CreateCoinStake(...)
```

### Regression test

```text
src/wallet/test/wallet_tests.cpp
coinstake_disconnect_releases_staked_input
```

### Minting GUI

```text
src/qt/mintingtablemodel.cpp
MintingTableModel::updateAge()
MintingTablePriv::refreshWallet()
```

### Remaining transaction GUI freeze

```text
src/qt/walletmodel.cpp
WalletModel::pollBalanceChanged()

src/qt/transactiontablemodel.cpp
TransactionTableModel::updateConfirmations()

src/qt/transactionview.cpp
TransactionFilterProxy
```

### Consensus/current-state follow-ups

```text
src/validation.h
DEFAULT_MAX_TIP_AGE

src/validation.cpp
bad-diffbits check
GetProofOfStakeReward()
```

---

## 43. Final state at end of handoff

As of the morning of September 26, 2026:

- PR #37 merged.
- PR #38 merged.
- PR #39 merged.
- PR #40 merged.
- `main` contains the final staking lock fix.
- The merged staking code was deployed beyond the original Mac to other significant staking nodes.
- Overnight observed staking behavior was materially cleaner.
- No new visible `?` stakes were present in the tested wallet screenshot the next morning.
- Explorer difficulty and estimated network participation had recovered substantially from the earlier low period.
- macOS DMG packaging works through the Rosetta x86_64 workflow.
- README badges are live.
- Linux deployment builds may require dependency cleanup, especially stale `/usr/local` Boost.
- Remaining major user-visible issue: transaction-list GUI beachball.
- Remaining major consensus-risk issue: commented-out `bad-diffbits` rejection.
- Remaining synchronization-policy issue: 2000-day `DEFAULT_MAX_TIP_AGE`.
- ~6 SUM PoS reward should be revisited separately as an economic/consensus decision.

---

## 44. Suggested commit for this document

After copying this file into the repository root:

```bash
cd ~/sumcoin
git switch main
git pull --ff-only origin main
git status --short
git diff --check
git add Sept_25_26_changes.md
git commit -m "docs: record September 25-26 engineering changes"
git push origin main
```

This document is documentation-only and should not alter source behavior.

---

## 45. One-paragraph handoff for an AI that only has 30 seconds

Sumcoin's main September 25-26 work fixed three classes of problems: wallet state after orphaned coinstakes (PR #37), catch-up/minter/GUI refresh behavior (PR #39), and a serious inherited lock-heavy PoS staking architecture (PR #40). PR #40 is the most important: large wallets could hold `cs_main` and `cs_wallet` through very long kernel scans, delaying network/wallet processing and creating avoidable stale-parent attempts. The final code searches outside both long-lived locks, briefly locks only chain-sensitive kernel checks and wallet key lookup, then revalidates the parent and selected inputs before signing/accepting a result. Live logs showed incoming blocks being processed during long stake searches, stale-search retries firing correctly, and the previous multi-minute tip-processing bursts disappearing in the final test. Genuine PoS forks can still create `?` stakes. Remaining short macOS beachballs were separately traced to `WalletModel::pollBalanceChanged()` -> `TransactionTableModel::updateConfirmations()` -> Qt proxy sorting/filtering on a huge transaction model. Do not mix that GUI fix into staking code. Also do not casually uncomment the current `bad-diffbits` consensus rejection, change the ~6 SUM reward, or assume the 2000-day `DEFAULT_MAX_TIP_AGE` is harmless.
