#include <cstring>
#include <string>

#include <boost/assign/list_of.hpp>

#include "constants.h"
#include "testnet.h"

static uint8_t      TESTNET_PROTOCOL_MAGIC_BYTES[sizeof(PROTOCOL_MAGIC_BYTES)] = { 0xe5, 0xcf, 0x81, 0xdf };

static char const * TESTNET_AUTO_DNS_SEEDS[][2]                                = { { "t-seed-a", "t-seed-a.neucoin.org" }, { 0, 0 } };
static uint32_t     TESTNET_AUTO_IP_SEEDS[]                                    = { 0 };

void ApplyTestnetParameters(void)
{
    COIN_PORT                  = 8742;
    RPC_PORT                   = 8743;

    std::memcpy(PROTOCOL_MAGIC_BYTES, &TESTNET_PROTOCOL_MAGIC_BYTES, sizeof(PROTOCOL_MAGIC_BYTES));

    AUTO_DNS_SEEDS             = TESTNET_AUTO_DNS_SEEDS;
    AUTO_IP_SEEDS              = TESTNET_AUTO_IP_SEEDS;

    PUBKEY_ADDRESS_PREFIX      =  65; // 'T'
    PRVKEY_ADDRESS_PREFIX      =  68; // 'U'
    SCRIPT_ADDRESS_PREFIX      = 127; // 't'

    GENESIS_MERKLE_HASH        = hash_t("0xa7fc40033828e0b0aa5f829ad35233efe8a15e5820d31e53d8e865a1c29cf024");
    GENESIS_HASH               = hash_t("0x000002f38bfabee423e760cbbbd2147984be70a531928b89e64d59531b136944");
    GENESIS_IDENT              = "06-15-15 :: LFG 887 :: We can still take them out for an overpriced dinner";
    GENESIS_TX_TIME            = 1345083810;
    GENESIS_BLOCK_TIME         = 1345084287;
    GENESIS_BLOCK_NONCE        = 2179980296;
    GENESIS_BLOCK_VERSION      = 1;

    STAKE_MODIFIER_CHECKPOINTS = boost::assign::map_list_of(0, 0xfd11f4e7);

    POW_MAX_BLOCK              = std::numeric_limits< blockheight_t >::max();

    COIN_PREMINE               = 0 * COIN;

    POW_INITIAL_TARGET         = target_t(~uint256(0) >> 20);
    POS_INITIAL_TARGET         = target_t(~uint256(0) >> 20);

    POW_MAX_TARGET             = target_t(~uint256(0) >> 20);
    POS_MAX_TARGET             = target_t(~uint256(0) >> 20);

    POW_TARGET_SPACING         =  1 * MINUTE;
    POS_TARGET_SPACING         = 30 * SECOND;

    POW_BLOCK_REWARD           = 1141 * COIN;

    POW_DUMMY_BLOCK_COUNT      = 0;

    STAKE_MIN_AGE              = 138240 * SECOND / 2;
    STAKE_MAX_AGE              = 138240 * SECOND / 2;

    MODIFIER_INTERVAL_BASE     = 200 * MINUTE / 2;
    TARGET_TIMESPAN            = 1 * HOUR ;

    BLOCK_CHECKPOINTS          = boost::assign::map_list_of(0, GENESIS_HASH);
}
