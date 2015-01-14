#pragma once

#include <limits>
#include <stdint.h>

#include "bignum.h"
#include "types.h"

#define                    COIN_NAME              "NeuCoin"
#define                    COIN_SLUG              "neucoin"
#define                    COIN_CLIENT_QT         "neucoin-qt"
#define                    COIN_CLIENT            "neucoind"
#define                    COIN_SCHEME            "neucoin"
#define                    COIN_DOMAIN            "neucoin.org"

#define                    COIN_UNIT              "NEU"
#define                    COIN_UNIT_M            "mNEU"
#define                    COIN_UNIT_U            "uNEU"

#define                    COIN_UNIT_NAME         "NeuCoins"
#define                    COIN_UNIT_NAME_M       "Milli-" COIN_UNIT
#define                    COIN_UNIT_NAME_U       "Micro-" COIN_UNIT

#define                    COIN_BUGTRACKER        "http://github.com/neucoin/neucoind/issues"

#define                    COIN_PORT               7742
#define                    RPC_PORT                7743

//                         These bytes are put at the beginning of every packet, and should be unique to a coin network
#define                    PROTOCOL_MAGIC_BYTES    { 0xe5, 0xcf, 0x81, 0xde }

//                         These parameters are used as initial nodes to connect to, from which we will get the other ones
#define                    AUTO_DNS_SEEDS          { }
#define                    AUTO_IP_SEEDS           { IP_ADDRESS( 5, 196, 149, 215 ) }

//                         These numbers each map to a base58 character which is put in front of the addresses to easily deduce an address type
#define                    PUBKEY_ADDRESS_PREFIX    53 // "N"
#define                    PRVKEY_ADDRESS_PREFIX    52 // "M"
#define                    SCRIPT_ADDRESS_PREFIX   112 // "n"

//                         The following constants define the genesis block of the coin network
//                         If you change them, the hash will probably be wrong and the genesis invalid (because its hash would be higher than the initial target), and the client will try to generate a new valid genesis at startup
static hash_t        const GENESIS_HASH           = hash_t("0x000009b4cc9d78dd3d3da01579414ddeb90ffe1d8fdf1fc232f16e174aef8724");
static char          const GENESIS_IDENT[]        = "Matonis 07-AUG-2012 Parallel Currencies And The Roadmap To Monetary Freedom";
static timestamp_t   const GENESIS_TX_TIME        = 1345083810;
static timestamp_t   const GENESIS_BLOCK_TIME     = 1345084287;
static uint32_t      const GENESIS_BLOCK_NONCE    = 2179786789;
static uint32_t      const GENESIS_BLOCK_VERSION  = 1;

//                         The maturity is the number of block required for a transaction to be confirmed by the network (excluding the block which embeds the transaction)
//                         Since you need to include your transaction in a block, and the COINBASE_MATURITY cannot be lower than 1, you will always need at least two blocks before maturing
static blockheight_t const COINBASE_MATURITY      = 1;

//                         Some parameters about the coin amount itself
//                         If you use a non-zero COIN_PREMINE value, the first mined PoW block will have this reward. Otherwise, it will be POW_BLOCK_REWARD as usual
static money_t       const MAX_MONEY              = 500000000000 * COIN;
static money_t       const COIN_PREMINE           =   2000000000 * COIN;

//                         If you submit a transaction to the network, you need to put at least MIN_TX_FEES coins for the client to accept it
//                         You also need to put at least MIN_RELAY_TX_FEES, otherwise the other nodes will not relay it -
static money_t       const MIN_TX_FEES            = CENT;
static money_t       const MIN_RELAY_TX_FEES      = CENT;

//                         The maximal amount of time drift allowed, after which the client will not accept old blocks
static timestamp_t   const MAX_CLOCK_DRIFT        = 2 * HOUR;

//                         The maximal size of a block allowed by your coin network
static uint32_t      const MAX_BLOCK_SIZE         = 1000000;

//                         Maximal number of PoW blocks, after which their reward become null
//                         You can use std::numeric_limits< blockheight_t >::max( ) to disable this parameter
static blockheight_t const POW_MAX_BLOCK          = std::numeric_limits< blockheight_t >::max( );

//                         Initial network targets
static target_t      const POW_INITIAL_TARGET     = target_t(~uint256(0) >> 20);
static target_t      const POS_INITIAL_TARGET     = target_t(~uint256(0) >> 20);

//                         Maximal network targets (after which mining/minting a block won't be easier)
static target_t      const POW_MAX_TARGET         = target_t(~uint256(0) >> 20);
static target_t      const POS_MAX_TARGET         = target_t(~uint256(0) >> 20);

//                         The average delay between two blocks
static timestamp_t   const POW_TARGET_SPACING     = 1 * MINUTE;
static timestamp_t   const POS_TARGET_SPACING     = 1 * MINUTE;

//                         Reward for each PoW block mined, until POW_MAX_BLOCK (from which it will become null)
static money_t       const POW_BLOCK_REWARD       = 1000 * COIN;

//                         The delay required for a coin to stake, and the delay after which a coin won't get any more bonuses
static timestamp_t   const STAKE_MIN_AGE          = 3 * MINUTE;
static timestamp_t   const STAKE_MAX_AGE          = 3 * MINUTE;

//                         When calculating the coinage, we use STAKE_COIN_STEP and STAKE_AGE_STEP to respectively divide the coin number and coin age
static money_t       const STAKE_COIN_STEP        = 1 * COIN;
static timestamp_t   const STAKE_AGE_STEP         = 1 * DAY;

//                         The target doesn't change immediately to take the new work difficulty in account - it is actually spread over an amount of time
static timestamp_t   const TARGET_TIMESPAN        = 10 * MINUTE;

// -- Be careful not to commit the checkpoint master key if you happen to use it
// -- Note that you should probably use the -checkpointkey command line parameter instead of using a modified client
// -- In order to generate a new valid checkpoint key, you have to use the `makekeypair` rpc command - don't try to generate it yourself

static std::string   const CHECKPOINT_PUBLIC_KEY  = "042f43a2e1e8185eb0f25380f4caa6de24d6a638ceb01374450207c9d25bfa586a20cc1295c686d20e090b324054aa4b93b405c9fea50df8bca0b70cca6b6758fc";
static std::string   const CHECKPOINT_PRIVATE_KEY = ""; // don't put the private key here, use -checkpointkey instead

// -- These variables should probably not be modified, since they rely on the previous ones

static money_t       const MIN_TXOUT_AMOUNT       = MIN_TX_FEES;

static uint32_t      const MAX_BLOCK_SIZE_GEN     = MAX_BLOCK_SIZE / 2;
static uint32_t      const MAX_BLOCK_SIGOPS       = MAX_BLOCK_SIZE / 50;
static uint32_t      const MAX_BLOCK_ORPHAN_TX    = MAX_BLOCK_SIZE / 100;

// -- And the following is just miscellaneous macro tools - please don't add any more except if it's for a really good reason

#define CONSTANT_TO_STRING(SYMBOL) TO_STRING(SYMBOL)
#define TO_STRING(WHAT) #WHAT
