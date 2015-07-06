export var fastChain = {

    GENESIS_MERKLE_HASH        : `hash_t("0xde076db12875d7472d677f11d8d3289c4ceae9bb3d0951a1fde6aed712e36311")`,
    GENESIS_HASH               : `hash_t("0xa2437a6611e8e0554ceb7e4d42ce25f31a849ae3e4b48b5a31bf969e4bec928a")`,
    GENESIS_TIME               : `1345084287`,
    GENESIS_NONCE              : `1043276304`,

    STAKE_MODIFIER_CHECKPOINTS : `boost::assign::map_list_of(0, 0xfd11f4e7)`,

    POW_INITIAL_TARGET         : `target_t(~uint256(0))`,
    POW_MAX_TARGET             : `target_t(~uint256(0))`,

    POW_TARGET_SPACING         : `1 * MINUTE`,
    POS_TARGET_SPACING         : `1 * MINUTE`,

    COINBASE_MATURITY          : `1`

};

export var smallChain = {

    COIN_PREMINE      : `0 * COIN`,
    POW_BLOCK_REWARD  : `1 * COIN`,

    MIN_TX_FEES       : `0 * COIN`,
    MIN_RELAY_TX_FEES : `0 * COIN`

};
