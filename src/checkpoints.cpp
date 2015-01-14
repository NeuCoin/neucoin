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
    typedef std::map<int, hash_t> MapCheckpoints;   // hardened checkpoints

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0,     GENESIS_HASH )
        ;

    bool CheckHardened( blockheight_t nHeight, hash_t hash) {

        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);

        if (i == mapCheckpoints.end())
            return true;

        return hash == i->second;

    }

    int GetTotalBlocksEstimate() {

        return mapCheckpoints.rbegin()->first;

    }

    CBlockIndex * GetLastCheckpoint(std::map<hash_t, CBlockIndex *> const & mapBlockIndex) {

        BOOST_REVERSE_FOREACH(MapCheckpoints::value_type const & i, mapCheckpoints) {

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
            return mapBlockIndex[hashSyncCheckpoint];
        }

        return NULL;

    }

    // ppcoin: only descendant of current sync-checkpoint is allowed
    bool ValidateSyncCheckpoint(hash_t hashCheckpoint) {

        if (!mapBlockIndex.count(hashSyncCheckpoint))
            return error("ValidateSyncCheckpoint: block index missing for current sync-checkpoint %s", hashSyncCheckpoint.ToString().c_str());

        if (!mapBlockIndex.count(hashCheckpoint))
            return error("ValidateSyncCheckpoint: block index missing for received sync-checkpoint %s", hashCheckpoint.ToString().c_str());

        CBlockIndex * pindexSyncCheckpoint = mapBlockIndex[hashSyncCheckpoint];
        CBlockIndex * pindexCheckpointRecv = mapBlockIndex[hashCheckpoint];

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
        CBlockIndex* pindexCheckpoint = mapBlockIndex[hashPendingCheckpoint];

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
        CBlockIndex const * pindexSync = mapBlockIndex[hashSyncCheckpoint];

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

        hash_t hash = mapCheckpoints.rbegin()->second;

        if (mapBlockIndex.count(hash) && !mapBlockIndex[hash]->IsInMainChain()) {

            // checkpoint block accepted but not yet in main chain
            printf("ResetSyncCheckpoint: SetBestChain to hardened checkpoint %s\n", hash.ToString().c_str());

            CTxDB txdb;
            CBlock block;

            if (!block.ReadFromDisk(mapBlockIndex[hash]))
                return error("ResetSyncCheckpoint: ReadFromDisk failed for hardened checkpoint %s", hash.ToString().c_str());

            if (!block.SetBestChain(txdb, mapBlockIndex[hash]))
                return error("ResetSyncCheckpoint: SetBestChain failed for hardened checkpoint %s", hash.ToString().c_str());

            txdb.Close();

        } else if(!mapBlockIndex.count(hash)) {

            // checkpoint block not yet accepted
            hashPendingCheckpoint = hash;
            checkpointMessagePending.SetNull();

            printf("ResetSyncCheckpoint: pending for sync-checkpoint %s\n", hashPendingCheckpoint.ToString().c_str());

        }

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints) {

            hash_t hash = i.second;

            if (mapBlockIndex.count(hash) && mapBlockIndex[hash]->IsInMainChain()) {

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

        while (ERR_get_error());
        printf("Errors have been reset\n");

        key.SetPrivKey(CPrivKey(vchPrivKey.begin(), vchPrivKey.end())); // if key is not correct openssl may crash

        for (int error; error = ERR_get_error(); )
            printf("%s\n", ERR_error_string(error, NULL));

        printf( "HELLOWORLD[%s]\n", strPrivKey.c_str());

        if (!key.Sign(Hash(checkpoint.vchMsg.begin(), checkpoint.vchMsg.end()), checkpoint.vchSig))
            return false;

        // Test signing successful, proceed
        CSyncCheckpoint::strMasterPrivKey = strPrivKey;

        return true;

    }

    bool SendSyncCheckpoint(hash_t hashCheckpoint) {

        printf("yup\n");

        if (CSyncCheckpoint::strMasterPrivKey.empty())
            return error("SendSyncCheckpoint: Checkpoint master key unavailable.");

        CSyncCheckpoint checkpoint;
        checkpoint.hashCheckpoint = hashCheckpoint;

        CDataStream sMsg(SER_NETWORK, PROTOCOL_VERSION);
        sMsg << (CUnsignedSyncCheckpoint)checkpoint;
        checkpoint.vchMsg = std::vector<unsigned char>(sMsg.begin(), sMsg.end());

        std::vector<unsigned char> vchPrivKey = ParseHex(CSyncCheckpoint::strMasterPrivKey);

        printf("yop\n");

        CKey key;
        key.SetPrivKey(CPrivKey(vchPrivKey.begin(), vchPrivKey.end())); // if key is not correct openssl may crash

        if (!key.Sign(Hash(checkpoint.vchMsg.begin(), checkpoint.vchMsg.end()), checkpoint.vchSig))
            return error("SendSyncCheckpoint: Unable to sign checkpoint, check private key?");

        if(!checkpoint.ProcessSyncCheckpoint(NULL)) {
            printf("WARNING: SendSyncCheckpoint: Failed to process checkpoint.\n");
            return false;
        }

        printf( "Switched checkpoint o/\n" );

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

        CBlockIndex const * pindexSync = mapBlockIndex[hashSyncCheckpoint];

        return (nBestHeight >= pindexSync->nHeight + COINBASE_MATURITY || pindexSync->GetBlockTime() + STAKE_MIN_AGE < GetAdjustedTime());

    }

    // Is the sync-checkpoint too old?
    bool IsSyncCheckpointTooOld(unsigned int nSeconds) {

        LOCK(cs_hashSyncCheckpoint);

        // sync-checkpoint should always be accepted block
        assert(mapBlockIndex.count(hashSyncCheckpoint));

        const CBlockIndex* pindexSync = mapBlockIndex[hashSyncCheckpoint];

        return (pindexSync->GetBlockTime() + nSeconds < GetAdjustedTime());

    }
}

// ppcoin: sync-checkpoint master key
const std::string CSyncCheckpoint::strMasterPubKey = CHECKPOINT_PUBLIC_KEY;
std::string CSyncCheckpoint::strMasterPrivKey = CHECKPOINT_PRIVATE_KEY;

// ppcoin: verify signature of sync-checkpoint message
bool CSyncCheckpoint::CheckSignature() {

    CKey key;

    if (!key.SetPubKey(ParseHex(CSyncCheckpoint::strMasterPubKey)))
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
    CBlockIndex* pindexCheckpoint = mapBlockIndex[hashCheckpoint];

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
