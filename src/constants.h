#pragma once

#include <limits>
#include <stdint.h>

#include "bignum.h"
#include "types.h"

#define COIN_NAME        "NeuCoin"
#define COIN_SLUG        "neucoin"
#define COIN_CLIENT_QT   "neucoin-qt"
#define COIN_CLIENT      "neucoind"
#define COIN_SCHEME      "neucoin"
#define COIN_DOMAIN      "neucoin.org"

#define COIN_UNIT        "NEU"
#define COIN_UNIT_M      "mNEU"
#define COIN_UNIT_U      "uNEU"

#define COIN_UNIT_NAME   "NeuCoins"
#define COIN_UNIT_NAME_M "Milli-" COIN_UNIT
#define COIN_UNIT_NAME_U "Micro-" COIN_UNIT

#define COIN_BUGTRACKER  "http://github.com/neucoin/neucoind/issues"

extern uint16_t      COIN_PORT;
extern uint16_t      RPC_PORT;

extern uint8_t       PROTOCOL_MAGIC_BYTES[4];

extern char const * (*AUTO_DNS_SEEDS)[2];
extern uint32_t     (*AUTO_IP_SEEDS);

extern uint8_t       PUBKEY_ADDRESS_PREFIX;
extern uint8_t       PRVKEY_ADDRESS_PREFIX;
extern uint8_t       SCRIPT_ADDRESS_PREFIX;

extern hash_t        GENESIS_MERKLE_HASH;
extern hash_t        GENESIS_HASH;
extern char const *  GENESIS_IDENT;
extern timestamp_t   GENESIS_TX_TIME;
extern timestamp_t   GENESIS_BLOCK_TIME;
extern uint32_t      GENESIS_BLOCK_NONCE;
extern uint32_t      GENESIS_BLOCK_VERSION;

extern std::map<blockheight_t, hash_t>   BLOCK_CHECKPOINTS;
extern std::map<blockheight_t, uint32_t> STAKE_MODIFIER_CHECKPOINTS;

extern blockheight_t COINBASE_MATURITY;

extern money_t       MAX_MONEY_STACK;
extern money_t       COIN_PREMINE;

extern money_t       MIN_TX_FEES;
extern money_t       MIN_RELAY_TX_FEES;

extern timestamp_t   MAX_CLOCK_DRIFT;

extern uint32_t      MAX_BLOCK_SIZE;

extern blockheight_t POW_MAX_BLOCK;

extern target_t      POW_INITIAL_TARGET;
extern target_t      POS_INITIAL_TARGET;

extern target_t      POW_MAX_TARGET;
extern target_t      POS_MAX_TARGET;

extern timestamp_t   POW_TARGET_SPACING;
extern timestamp_t   POS_TARGET_SPACING;

extern money_t       POW_BLOCK_REWARD;

extern timestamp_t   STAKE_MIN_AGE;
extern timestamp_t   STAKE_MAX_AGE;

extern money_t       STAKE_COIN_STEP;
extern timestamp_t   STAKE_AGE_STEP;

extern timestamp_t   TARGET_TIMESPAN;

extern std::string   CHECKPOINT_PUBLIC_KEY;
extern std::string   CHECKPOINT_PRIVATE_KEY;

extern money_t       MIN_TXOUT_AMOUNT;

extern uint32_t      MAX_BLOCK_SIZE_GEN;
extern uint32_t      MAX_BLOCK_SIGOPS;
extern uint32_t      MAX_BLOCK_ORPHAN_TX;

// -- And the following is just miscellaneous macro tools - please don't add any more except if it's for a really good reason

#define CONSTANT_TO_STRING(SYMBOL) TO_STRING(SYMBOL)
#define TO_STRING(WHAT) #WHAT
