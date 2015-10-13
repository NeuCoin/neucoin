// Copyright (c) 2012-2013 The PPCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef PPCOIN_KERNEL_H
#define PPCOIN_KERNEL_H

#include "StakeStats.h"
#include "main.h"

typedef std::pair<COutPoint, unsigned int> pos_kernel_t;

// Blacklist a kernel, and link a block hash to it
void BlacklistProofOfStake(pos_kernel_t const & proofOfStake, hash_t const & blockHash);

// Remove a kernel from the blacklist, with all its associated blocks
void CleanProofOfStakeBlacklist(pos_kernel_t const & proofOfStake);
void CleanProofOfStakeBlacklist(hash_t const & blockHash);

// Check if a kernel or a block has been blacklisted
bool IsProofOfStakeBlacklisted(pos_kernel_t const & proofOfStake);
bool IsProofOfStakeBlacklisted(hash_t const & blockHash);

int64 GetStakeModifierSelectionInterval(void);

// Compute the hash modifier for proof-of-stake
bool ComputeNextStakeModifier(const CBlockIndex* pindexCurrent, uint64& nStakeModifier, bool& fGeneratedStakeModifier);

// Check whether stake kernel meets hash target
// Sets hashProofOfStake on success return
bool CheckStakeKernelHash(const CBlockIndex * pindexPrev, unsigned int nBits, const CBlock& blockFrom, unsigned int nTxPrevOffset, const CTransaction& txPrev, const COutPoint& prevout, unsigned int nTimeTx, uint256& hashProofOfStake, bool fPrintProofOfStake=false, StakeStats * stakeStats = NULL);

// Check kernel hash target and coinstake signature
// Sets hashProofOfStake on success return
bool CheckProofOfStake(const CBlockIndex * pindexPrev, const CTransaction& tx, unsigned int nBits, uint256& hashProofOfStake);

// Check whether the coinstake timestamp meets protocol
bool CheckCoinStakeTimestamp(int64 nTimeBlock, int64 nTimeTx);

// Get stake modifier checksum
unsigned int GetStakeModifierChecksum(const CBlockIndex* pindex);

// Check stake modifier hard checkpoints
bool CheckStakeModifierCheckpoints(int nHeight, unsigned int nStakeModifierChecksum);

#endif // PPCOIN_KERNEL_H
