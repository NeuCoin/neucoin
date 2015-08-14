#include <limits>
#include <map>
#include <stdint.h>

#include <boost/assign/list_of.hpp>

#include "bignum.h"
#include "macros.h"
#include "types.h"

uint16_t                          COIN_PORT                   = 7742;
uint16_t                          RPC_PORT                    = 7743;

//                                These bytes are put at the beginning of every packet, and should be unique to a coin network
//                                They are designed to be unlikely to occur in normal data. The characters are rarely used upper ascii, not valid as UTF-8, and produce a large 4-byte int at any alignment

uint8_t                           PROTOCOL_MAGIC_BYTES[4]     = { 0xe5, 0xcf, 0x81, 0xde };

//                                These parameters are used as initial nodes to connect to, from which we will get the other ones
//                                Each DNS pair gives a source name and a seed name. The first name is used as information source for addrman, and the second name should resolve to a list of seed addresses. The last element of each array has to be null (respectively { 0, 0 } and 0)
//                                A short note about the syntax : we have to use a pointer-to-array (rather than array-of-array-of-pointers) because we need to be able to reassign it later and change the array size (testnet). I'd like to improve that latter (and probably use a std::list instead)

//                                Neucoin-specific : "seed-a" & "seed-b" are full nodes, "seeds" is a seed server

static char const *               MAINNET_AUTO_DNS_SEEDS[][2] = { { "seed-a", "seed-a.neucoin.org" }, { "seed-b", "seed-b.neucoin.org" }, { "seeds", "seeds.for.neucoin.org" }, { 0, 0 } };
static uint32_t                   MAINNET_AUTO_IP_SEEDS[]     = { 0 };

char const *                    (*AUTO_DNS_SEEDS)[2]          = MAINNET_AUTO_DNS_SEEDS;
uint32_t                        (*AUTO_IP_SEEDS)              = MAINNET_AUTO_IP_SEEDS;

//                                These numbers each map to a base58 character which is put in front of the addresses to easily deduce an address type

uint8_t                           PUBKEY_ADDRESS_PREFIX       =  53; // "N"
uint8_t                           PRVKEY_ADDRESS_PREFIX       =  52; // "M"
uint8_t                           SCRIPT_ADDRESS_PREFIX       = 112; // "n"

//                                The following constants define the genesis block of the coin network
//                                If you change them, the hash will probably be wrong and the genesis invalid (because its hash would be higher than the initial target), and the client will try to generate a new valid genesis at startup

hash_t                            GENESIS_MERKLE_HASH         = hash_t("0xde076db12875d7472d677f11d8d3289c4ceae9bb3d0951a1fde6aed712e36311");
hash_t                            GENESIS_HASH                = hash_t("0x0000052bba6df3f7601a0cfdfdd4eab14072e66fbabd6870074f92a9a59079b5");
char const *                      GENESIS_IDENT               = "06-12-15 :: XKCD 1537 :: My new language is great, but it has a few quirks regarding type";
timestamp_t                       GENESIS_TX_TIME             = 1345083810;
timestamp_t                       GENESIS_BLOCK_TIME          = 1345084287;
uint32_t                          GENESIS_BLOCK_NONCE         = 1043276304;
uint32_t                          GENESIS_BLOCK_VERSION       = 1;

std::map<blockheight_t, hash_t>   BLOCK_CHECKPOINTS           = boost::assign::map_list_of(0, GENESIS_HASH)(1, "0x000008bf53ebd8a2f33f3147579f9988b56523c6622c45499dd812d06593bf8d")((8 * WEEK) / (10 * MINUTE) + (8 * WEEK) / (1 * MINUTE), "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
std::map<blockheight_t, uint32_t> STAKE_MODIFIER_CHECKPOINTS  = boost::assign::map_list_of(0, 0x0e00670b);

//                                The maturity is the number of block required for a coinbase/coinstake transaction to be confirmed by the network (excluding the block which embeds the transaction)
//                                Since you need to include your transaction in a block, and the COINBASE_MATURITY cannot be lower than 1, you will always need at least two blocks before maturing
//                                Note that the coinbase maturity is actually also applied to coinstakes. This constant should probably be renamed to reflect this fact

blockheight_t                     COINBASE_MATURITY           = 500;

//                                Some parameters about the coin amount itself
//                                If you use a non-zero COIN_PREMINE value, the first mined PoW block will have this reward. Otherwise, it will be POW_BLOCK_REWARD as usual
//                                Note that MAX_MONEY_STACK doesn't prevent the coin amount from exceeding its value - ie. that it isn't a hard limit for how much money will the blockchain handle

money_t                           MAX_MONEY_STACK             = 500000000000 * COIN;
money_t                           COIN_PREMINE                =   3000000000 * COIN;

//                                If you submit a transaction to the network, you need to put at least MIN_TX_FEES coins for the client to accept it
//                                You also need to put at least MIN_RELAY_TX_FEES, otherwise the other nodes will not relay it -

money_t                           MIN_TX_FEES                 = CENT;
money_t                           MIN_RELAY_TX_FEES           = CENT;

//                                The maximal amount of time drift allowed, after which the client will not accept old blocks

timestamp_t                       MAX_CLOCK_DRIFT             = 2 * HOUR;

//                                The maximal size of a block allowed by your coin network

uint32_t                          MAX_BLOCK_SIZE              = 1000000;

//                                Maximal number of PoW blocks, after which their reward become null
//                                You can use std::numeric_limits< blockheight_t >::max( ) to disable this parameter

blockheight_t                     POW_MAX_BLOCK               = 52596;

//                                Initial network targets

target_t                          POW_INITIAL_TARGET          = target_t(~uint256(0) >> 20);
target_t                          POS_INITIAL_TARGET          = target_t(~uint256(0) >> 20);

//                                Maximal network targets (after which mining/minting a block won't be easier)

target_t                          POW_MAX_TARGET              = target_t(~uint256(0) >> 20);
target_t                          POS_MAX_TARGET              = target_t(~uint256(0) >> 20);

//                                The average delay between two blocks

timestamp_t                       POW_TARGET_SPACING          = 10 * MINUTE;
timestamp_t                       POS_TARGET_SPACING          = 1 * MINUTE;

//                                Reward for each PoW block mined, until POW_MAX_BLOCK (from which it will become null)

money_t                           POW_BLOCK_REWARD            = 1141 * COIN;

//                                The delay required for a coin to stake, and the delay after which a coin won't get any more bonuses

timestamp_t                       STAKE_MIN_AGE               = 138240 * SECOND; // = 1.6 days, which is a bit higher than the stake modifier selection interval (1.56 days)
timestamp_t                       STAKE_MAX_AGE               = 138240 * SECOND;

//                                These variable define the amount of time a stake modifier spreads over. The BASE is the delay to compute the first bit of a modifier, and the RATIO is the multiplicator for the last bit.
//                                As many other things, these variables were very scarcely documented, so they might be inaccurate. Please open a PR to fix this comment if you understand them better than I.

timestamp_t                       MODIFIER_INTERVAL_BASE      = 200 * MINUTE;
uint32_t                          MODIFIER_INTERVAL_RATIO     = 18;

//                                When calculating the coinage, we use STAKE_COIN_STEP and STAKE_AGE_STEP to respectively divide the coin number and coin age

money_t                           STAKE_COIN_STEP             = 1 * COIN;
timestamp_t                       STAKE_AGE_STEP              = 1 * DAY;

//                                The target doesn't change immediately to take the new work difficulty in account - it is actually spread over an amount of time, the TARGET_TIMESPAN

timestamp_t                       TARGET_TIMESPAN             = 2 * HOUR;

// -- You can use the -checkpointkey command line parameter to specify the private key. If you do this, and if the private key is correct, your client will start sending checkpoints regularly
// -- In order to generate a new valid checkpoint key, you have to use the `makekeypair` rpc command - don't try to generate it yourself

std::string                       CHECKPOINT_PUBLIC_KEY       = "042f43a2e1e8185eb0f25380f4caa6de24d6a638ceb01374450207c9d25bfa586a20cc1295c686d20e090b324054aa4b93b405c9fea50df8bca0b70cca6b6758fc";
std::string                       CHECKPOINT_PRIVATE_KEY      = ""; // Do NOT set it here; use -checkpointkey instead

// -- These variables should probably not be modified, since they rely on the previous ones

money_t                           MIN_TXOUT_AMOUNT            = MIN_TX_FEES;

uint32_t                          MAX_BLOCK_SIZE_GEN          = MAX_BLOCK_SIZE / 2;
uint32_t                          MAX_BLOCK_SIGOPS            = MAX_BLOCK_SIZE / 50;
uint32_t                          MAX_BLOCK_ORPHAN_TX         = MAX_BLOCK_SIZE / 100;
