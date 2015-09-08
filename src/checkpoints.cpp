// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2013 The PPCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>
#include <openssl/err.h>

#include "checkpoints.h"
#include "db.h"
#include "macros.h"
#include "main.h"
#include "types.h"

namespace Checkpoints
{
    bool CheckHardened( blockheight_t nHeight, hash_t hash)
    {
        std::map<blockheight_t, hash_t>::const_iterator i = BLOCK_CHECKPOINTS.find(nHeight);

        if (i == BLOCK_CHECKPOINTS.end())
            return true;

        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        std::map<blockheight_t, hash_t>::const_reverse_iterator i = BLOCK_CHECKPOINTS.rbegin();

        // Allow the last checkpoint to be a "kill-checkpoint", that will prevent the blockchain from progressing further
        // In such a case, since this checkpoint is in the future, we don't count it towards the block count estimation

        if (i->second == ~hash_t(0))
            ++ i;

        assert(i != BLOCK_CHECKPOINTS.rend());

        return i->first;
    }

    CBlockIndex * GetLastCheckpoint(std::map<hash_t, CBlockIndex *> const & mapBlockIndex)
    {
        typedef std::map<blockheight_t, hash_t> checkpoints_t;

        BOOST_REVERSE_FOREACH(checkpoints_t::value_type const & i, BLOCK_CHECKPOINTS) {

            hash_t hash = i.second;
            std::map<hash_t, CBlockIndex *>::const_iterator t = mapBlockIndex.find(hash);

            if (t != mapBlockIndex.end()) {
                return t->second;
            }

        }

        return NULL;
    }

    // ppcoin: synchronized checkpoint (centrally broadcasted)
    hash_t hashSyncCheckpoint = 0;
    hash_t hashPendingCheckpoint = 0;
    hash_t hashInvalidCheckpoint = 0;

    CSyncCheckpoint checkpointMessage;
    CSyncCheckpoint checkpointMessagePending;

    CCriticalSection cs_hashSyncCheckpoint;

    // ppcoin: get last synchronized checkpoint
    CBlockIndex * GetLastSyncCheckpoint() {

        LOCK(cs_hashSyncCheckpoint);

        if (!mapBlockIndex.count(hashSyncCheckpoint)) {
            error("GetSyncCheckpoint: block index missing for current sync-checkpoint %s", hashSyncCheckpoint.ToString().c_str());
        } else {
            return mapBlockIndex.at(hashSyncCheckpoint);
        }

        return NULL;

    }

    // ppcoin: only descendant of current sync-checkpoint is allowed
    bool ValidateSyncCheckpoint(hash_t hashCheckpoint) {

        if (!mapBlockIndex.count(hashSyncCheckpoint))
            return error("ValidateSyncCheckpoint: block index missing for current sync-checkpoint %s", hashSyncCheckpoint.ToString().c_str());

        if (!mapBlockIndex.count(hashCheckpoint))
            return error("ValidateSyncCheckpoint: block index missing for received sync-checkpoint %s", hashCheckpoint.ToString().c_str());

        CBlockIndex * pindexSyncCheckpoint = mapBlockIndex.at(hashSyncCheckpoint);
        CBlockIndex * pindexCheckpointRecv = mapBlockIndex.at(hashCheckpoint);

        if (pindexCheckpointRecv->nHeight <= pindexSyncCheckpoint->nHeight) {

            // Received an older checkpoint, trace back from current checkpoint
            // to the same height of the received checkpoint to verify
            // that current checkpoint should be a descendant block
            CBlockIndex* pindex = pindexSyncCheckpoint;

            while (pindex->nHeight > pindexCheckpointRecv->nHeight)
                if (!(pindex = pindex->pprev))
                    return error("ValidateSyncCheckpoint: pprev1 null - block index structure failure");

            if (pindex->GetBlockHash() != hashCheckpoint) {
                hashInvalidCheckpoint = hashCheckpoint;
                return error("ValidateSyncCheckpoint: new sync-checkpoint %s is conflicting with current sync-checkpoint %s", hashCheckpoint.ToString().c_str(), hashSyncCheckpoint.ToString().c_str());
            }

            return false; // ignore older checkpoint

        }

        // Received checkpoint should be a descendant block of the current
        // checkpoint. Trace back to the same height of current checkpoint
        // to verify.
        CBlockIndex* pindex = pindexCheckpointRecv;
        while (pindex->nHeight > pindexSyncCheckpoint->nHeight)
            if (!(pindex = pindex->pprev))
                return error("ValidateSyncCheckpoint: pprev2 null - block index structure failure");

        if (pindex->GetBlockHash() != hashSyncCheckpoint) {
            hashInvalidCheckpoint = hashCheckpoint;
            return error("ValidateSyncCheckpoint: new sync-checkpoint %s is not a descendant of current sync-checkpoint %s", hashCheckpoint.ToString().c_str(), hashSyncCheckpoint.ToString().c_str());
        }

        return true;

    }

    bool WriteSyncCheckpoint(hash_t hashCheckpoint) {

        CTxDB txdb;
        txdb.TxnBegin();

        if (!txdb.WriteSyncCheckpoint(hashCheckpoint)) {
            txdb.TxnAbort();
            return error("WriteSyncCheckpoint(): failed to write to db sync checkpoint %s", hashCheckpoint.ToString().c_str());
        }

        if (!txdb.TxnCommit())
            return error("WriteSyncCheckpoint(): failed to commit to db sync checkpoint %s", hashCheckpoint.ToString().c_str());

        txdb.Close();

        Checkpoints::hashSyncCheckpoint = hashCheckpoint;

        return true;

    }

    bool AcceptPendingSyncCheckpoint() {

        LOCK(cs_hashSyncCheckpoint);

        if (hashPendingCheckpoint == 0 || ! mapBlockIndex.count(hashPendingCheckpoint))
            return false;

        if (!ValidateSyncCheckpoint(hashPendingCheckpoint)) {
            hashPendingCheckpoint = 0;
            checkpointMessagePending.SetNull();
            return false;
        }

        CTxDB txdb;
        CBlockIndex* pindexCheckpoint = mapBlockIndex.at(hashPendingCheckpoint);

        if (!pindexCheckpoint->IsInMainChain()) {

            CBlock block;

            if (!block.ReadFromDisk(pindexCheckpoint))
                return error("AcceptPendingSyncCheckpoint: ReadFromDisk failed for sync checkpoint %s", hashPendingCheckpoint.ToString().c_str());

            if (!block.SetBestChain(txdb, pindexCheckpoint)) {
                hashInvalidCheckpoint = hashPendingCheckpoint;
                return error("AcceptPendingSyncCheckpoint: SetBestChain failed for sync checkpoint %s", hashPendingCheckpoint.ToString().c_str());
            }

        }

        txdb.Close();

        if (!WriteSyncCheckpoint(hashPendingCheckpoint))
            return error("AcceptPendingSyncCheckpoint(): failed to write sync checkpoint %s", hashPendingCheckpoint.ToString().c_str());

        hashPendingCheckpoint = 0;
        checkpointMessage = checkpointMessagePending;
        checkpointMessagePending.SetNull();

        printf("AcceptPendingSyncCheckpoint : sync-checkpoint at %s\n", hashSyncCheckpoint.ToString().c_str());

        // relay the checkpoint
        if (!checkpointMessage.IsNull()) {
            BOOST_FOREACH(CNode* pnode, vNodes) {
                checkpointMessage.RelayTo(pnode);
            }
        }

        return true;

    }

    // Automatically select a suitable sync-checkpoint
    hash_t AutoSelectSyncCheckpoint() {

        // Proof-of-work blocks are immediately checkpointed
        // to defend against 51% attack which rejects other miners block

        // Select the last proof-of-work block
        const CBlockIndex *pindex = GetLastBlockIndex(pindexBest, false);

        // Search forward for a block within max span and maturity window
        while (pindex->pnext && (pindex->GetBlockTime() + CHECKPOINT_MAX_SPAN <= pindexBest->GetBlockTime() || pindex->nHeight + COINBASE_MATURITY <= pindexBest->nHeight))
            pindex = pindex->pnext;

        return pindex->GetBlockHash();

    }

    // Check against synchronized checkpoint
    bool CheckSync(hash_t hashBlock, CBlockIndex const * pindexPrev) {

        int nHeight = pindexPrev->nHeight + 1;

        LOCK(cs_hashSyncCheckpoint);

        // sync-checkpoint should always be accepted block
        assert(mapBlockIndex.count(hashSyncCheckpoint));
        CBlockIndex const * pindexSync = mapBlockIndex.at(hashSyncCheckpoint);

        if (nHeight > pindexSync->nHeight) {

            // trace back to same height as sync-checkpoint
            const CBlockIndex* pindex = pindexPrev;

            while (pindex->nHeight > pindexSync->nHeight)
                if (!(pindex = pindex->pprev))
                    return error("CheckSync: pprev null - block index structure failure");

            if (pindex->nHeight < pindexSync->nHeight || pindex->GetBlockHash() != hashSyncCheckpoint) {
                return false; // only descendant of sync-checkpoint can pass check
            }

        }

        if (nHeight == pindexSync->nHeight && hashBlock != hashSyncCheckpoint)
            return false; // same height with sync-checkpoint

        if (nHeight < pindexSync->nHeight && !mapBlockIndex.count(hashBlock))
            return false; // lower height than sync-checkpoint

        return true;

    }

    bool WantedByPendingSyncCheckpoint(hash_t hashBlock) {

        LOCK(cs_hashSyncCheckpoint);

        if (hashPendingCheckpoint == 0)
            return false;

        if (hashBlock == hashPendingCheckpoint)
            return true;

        if (mapOrphanBlocks.count(hashPendingCheckpoint) && hashBlock == WantedByOrphan(mapOrphanBlocks[hashPendingCheckpoint]))
            return true;

        return false;

    }

    // ppcoin: reset synchronized checkpoint to last hardened checkpoint
    bool ResetSyncCheckpoint() {

        LOCK(cs_hashSyncCheckpoint);

        hash_t hash = BLOCK_CHECKPOINTS.rbegin()->second;

        if (mapBlockIndex.count(hash) && !mapBlockIndex.at(hash)->IsInMainChain()) {

            // checkpoint block accepted but not yet in main chain
            printf("ResetSyncCheckpoint: SetBestChain to hardened checkpoint %s\n", hash.ToString().c_str());

            CTxDB txdb;
            CBlock block;

            if (!block.ReadFromDisk(mapBlockIndex.at(hash)))
                return error("ResetSyncCheckpoint: ReadFromDisk failed for hardened checkpoint %s", hash.ToString().c_str());

            if (!block.SetBestChain(txdb, mapBlockIndex.at(hash)))
                return error("ResetSyncCheckpoint: SetBestChain failed for hardened checkpoint %s", hash.ToString().c_str());

            txdb.Close();

        } else if(!mapBlockIndex.count(hash)) {

            // checkpoint block not yet accepted
            hashPendingCheckpoint = hash;
            checkpointMessagePending.SetNull();

            printf("ResetSyncCheckpoint: pending for sync-checkpoint %s\n", hashPendingCheckpoint.ToString().c_str());

        }

        typedef std::map<blockheight_t, hash_t> checkpoints_t;

        BOOST_REVERSE_FOREACH(const checkpoints_t::value_type& i, BLOCK_CHECKPOINTS) {

            hash_t hash = i.second;

            if (mapBlockIndex.count(hash) && mapBlockIndex.at(hash)->IsInMainChain()) {

                if (!WriteSyncCheckpoint(hash))
                    return error("ResetSyncCheckpoint: failed to write sync checkpoint %s", hash.ToString().c_str());

                printf("ResetSyncCheckpoint: sync-checkpoint reset to %s\n", hashSyncCheckpoint.ToString().c_str());
                return true;

            }

        }

        return false;
    }

    void AskForPendingSyncCheckpoint(CNode* pfrom) {

        LOCK(cs_hashSyncCheckpoint);

        if (pfrom && hashPendingCheckpoint != 0 && (!mapBlockIndex.count(hashPendingCheckpoint)) && (!mapOrphanBlocks.count(hashPendingCheckpoint))) {
            pfrom->AskFor(CInv(MSG_BLOCK, hashPendingCheckpoint));
        }

    }

    bool SetCheckpointPrivKey(std::string strPrivKey) {

        // Test signing a sync-checkpoint with genesis block
        CSyncCheckpoint checkpoint;
        checkpoint.hashCheckpoint = GENESIS_HASH;

        CDataStream sMsg(SER_NETWORK, PROTOCOL_VERSION);
        sMsg << (CUnsignedSyncCheckpoint)checkpoint;
        checkpoint.vchMsg = std::vector<unsigned char>(sMsg.begin(), sMsg.end());

        std::vector<unsigned char> vchPrivKey = ParseHex(strPrivKey);

        CKey key;

        while (ERR_get_error()) ; // Reset errors
        printf("Errors have been reset\n");

        key.SetPrivKey(CPrivKey(vchPrivKey.begin(), vchPrivKey.end())); // if key is not correct openssl may crash

        int error;
        while ((error = ERR_get_error()))
            printf("%s\n", ERR_error_string(error, NULL));

        if (!key.Sign(Hash(checkpoint.vchMsg.begin(), checkpoint.vchMsg.end()), checkpoint.vchSig))
            return false;

        // Test signing successful, proceed
        CHECKPOINT_PRIVATE_KEY = strPrivKey;

        return true;

    }

    bool SendSyncCheckpoint(hash_t hashCheckpoint) {

        if (CHECKPOINT_PRIVATE_KEY.empty())
            return error("SendSyncCheckpoint: Checkpoint master key unavailable.");

        CSyncCheckpoint checkpoint;
        checkpoint.hashCheckpoint = hashCheckpoint;

        CDataStream sMsg(SER_NETWORK, PROTOCOL_VERSION);
        sMsg << (CUnsignedSyncCheckpoint)checkpoint;
        checkpoint.vchMsg = std::vector<unsigned char>(sMsg.begin(), sMsg.end());

        std::vector<unsigned char> vchPrivKey = ParseHex(CHECKPOINT_PRIVATE_KEY);

        CKey key;
        key.SetPrivKey(CPrivKey(vchPrivKey.begin(), vchPrivKey.end())); // if key is not correct openssl may crash

        if (!key.Sign(Hash(checkpoint.vchMsg.begin(), checkpoint.vchMsg.end()), checkpoint.vchSig))
            return error("SendSyncCheckpoint: Unable to sign checkpoint, check private key?");

        if(!checkpoint.ProcessSyncCheckpoint(NULL)) {
            printf("WARNING: SendSyncCheckpoint: Failed to process checkpoint.\n");
            return false;
        }

        // Relay checkpoint
        {
            LOCK(cs_vNodes);
            BOOST_FOREACH(CNode* pnode, vNodes) {
                checkpoint.RelayTo(pnode);
            }
        }

        return true;

    }

    // Is the sync-checkpoint outside maturity window?
    bool IsSyncCheckpointMature() {

        LOCK(cs_hashSyncCheckpoint);

        // sync-checkpoint should always be accepted block
        assert(mapBlockIndex.count(hashSyncCheckpoint));

        CBlockIndex const * pindexSync = mapBlockIndex.at(hashSyncCheckpoint);

        return (nBestHeight >= pindexSync->nHeight + COINBASE_MATURITY || pindexSync->GetBlockTime() + STAKE_MIN_AGE < GetAdjustedTime());

    }

    // Is the sync-checkpoint too old?
    bool IsSyncCheckpointTooOld(unsigned int nSeconds) {

        LOCK(cs_hashSyncCheckpoint);

        // sync-checkpoint should always be accepted block
        assert(mapBlockIndex.count(hashSyncCheckpoint));

        const CBlockIndex* pindexSync = mapBlockIndex.at(hashSyncCheckpoint);

        return (pindexSync->GetBlockTime() + nSeconds < GetAdjustedTime());

    }
}

// ppcoin: verify signature of sync-checkpoint message
bool CSyncCheckpoint::CheckSignature() {

    CKey key;

    if (!key.SetPubKey(ParseHex(CHECKPOINT_PUBLIC_KEY)))
        return error("CSyncCheckpoint::CheckSignature() : SetPubKey failed");

    if (!key.Verify(Hash(vchMsg.begin(), vchMsg.end()), vchSig))
        return error("CSyncCheckpoint::CheckSignature() : verify signature failed");

    // Now unserialize the data
    CDataStream sMsg(vchMsg, SER_NETWORK, PROTOCOL_VERSION);
    sMsg >> *(CUnsignedSyncCheckpoint*)this;

    return true;

}

// ppcoin: process synchronized checkpoint
bool CSyncCheckpoint::ProcessSyncCheckpoint(CNode* pfrom) {

    if (!CheckSignature())
        return false;

    LOCK(Checkpoints::cs_hashSyncCheckpoint);

    if (!mapBlockIndex.count(hashCheckpoint)) {

        // We haven't received the checkpoint chain, keep the checkpoint as pending
        Checkpoints::hashPendingCheckpoint = hashCheckpoint;
        Checkpoints::checkpointMessagePending = *this;

        printf("ProcessSyncCheckpoint: pending for sync-checkpoint %s\n", hashCheckpoint.ToString().c_str());

        // Ask this guy to fill in what we're missing
        if (pfrom) {
            pfrom->PushGetBlocks(pindexBest, hashCheckpoint);
            // ask directly as well in case rejected earlier by duplicate
            // proof-of-stake because getblocks may not get it this time
            pfrom->AskFor(CInv(MSG_BLOCK, mapOrphanBlocks.count(hashCheckpoint)? WantedByOrphan(mapOrphanBlocks[hashCheckpoint]) : hashCheckpoint));
        }

        return false;

    }

    if (!Checkpoints::ValidateSyncCheckpoint(hashCheckpoint))
        return false;

    CTxDB txdb;
    CBlockIndex* pindexCheckpoint = mapBlockIndex.at(hashCheckpoint);

    if (!pindexCheckpoint->IsInMainChain()) {

        // checkpoint chain received but not yet main chain
        CBlock block;

        if (!block.ReadFromDisk(pindexCheckpoint))
            return error("ProcessSyncCheckpoint: ReadFromDisk failed for sync checkpoint %s", hashCheckpoint.ToString().c_str());

        if (!block.SetBestChain(txdb, pindexCheckpoint)) {
            Checkpoints::hashInvalidCheckpoint = hashCheckpoint;
            return error("ProcessSyncCheckpoint: SetBestChain failed for sync checkpoint %s", hashCheckpoint.ToString().c_str());
        }

    }

    txdb.Close();

    if (!Checkpoints::WriteSyncCheckpoint(hashCheckpoint))
        return error("ProcessSyncCheckpoint(): failed to write sync checkpoint %s", hashCheckpoint.ToString().c_str());

    Checkpoints::checkpointMessage = *this;
    Checkpoints::hashPendingCheckpoint = 0;
    Checkpoints::checkpointMessagePending.SetNull();

    printf("ProcessSyncCheckpoint: sync-checkpoint at %s\n", hashCheckpoint.ToString().c_str());

    return true;

}
