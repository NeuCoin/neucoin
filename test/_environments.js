export var fastChain = {

    GENESIS_MERKLE_HASH        : `hash_t("0x6cc1b3157225af10b4a9463c781500e42edfdd838552258c022c64a43bfeede4")`,
    GENESIS_HASH               : `hash_t("0xa23c0f252952dab41e98ba678a73f1844c872bd39d57df7851a01da3f112fb94")`,
    GENESIS_TIME               : `1441976184`,
    GENESIS_NONCE              : `172958`,

    BLOCK_CHECKPOINTS          : `boost::assign::map_list_of(0, GENESIS_HASH)`,
    STAKE_MODIFIER_CHECKPOINTS : `boost::assign::map_list_of(0, 0xfd11f4e7)`,

    POW_MAX_BLOCK              : `std::numeric_limits<blockheight_t>::max()`,

    POW_INITIAL_TARGET         : `target_t(~uint256(0))`,
    POS_INITIAL_TARGET         : `target_t(~uint256(0))`,

    POW_MAX_TARGET             : `target_t(~uint256(0))`,
    POS_MAX_TARGET             : `target_t(~uint256(0))`,

    POW_TARGET_SPACING         : `1 * SECOND`,
    POS_TARGET_SPACING         : `1 * SECOND`,

    STAKE_MIN_AGE              : `0 * SECOND`,
    STAKE_MAX_AGE              : `0 * SECOND`,

    STAKE_COIN_STEP            : `1`,
    STAKE_AGE_STEP             : `1`,

    COINBASE_MATURITY          : `1`

};

export var mediumFastChain = {

    GENESIS_MERKLE_HASH        : `hash_t("0x6cc1b3157225af10b4a9463c781500e42edfdd838552258c022c64a43bfeede4")`,
    GENESIS_HASH               : `hash_t("0xa23c0f252952dab41e98ba678a73f1844c872bd39d57df7851a01da3f112fb94")`,
    GENESIS_TIME               : `1441976184`,
    GENESIS_NONCE              : `172958`,

    BLOCK_CHECKPOINTS          : `boost::assign::map_list_of(0, GENESIS_HASH)`,
    STAKE_MODIFIER_CHECKPOINTS : `boost::assign::map_list_of(0, 0xfd11f4e7)`,

    POW_INITIAL_TARGET         : `target_t(~uint256(0))`,
    POS_INITIAL_TARGET         : `target_t(~uint256(0))`,

    POW_MAX_TARGET             : `target_t(~uint256(0))`,
    POS_MAX_TARGET             : `target_t(~uint256(0))`,

    POW_TARGET_SPACING         : `6 * SECOND`,
    POS_TARGET_SPACING         : `6 * SECOND`,

    STAKE_MIN_AGE              : `0 * SECOND`,
    STAKE_MAX_AGE              : `0 * SECOND`,

    TARGET_TIMESPAN            : `1 * MINUTE`,

    COINBASE_MATURITY          : `1`

};

export var smallChain = {

    COIN_PREMINE          : `0 * COIN`,

    POW_DUMMY_BLOCK_COUNT : `0`,

    POW_BLOCK_REWARD      : `1 * COIN`,
    POS_BLOCK_REWARD      : `1 * COIN`,

    MIN_TX_FEES           : `0 * COIN`,
    MIN_RELAY_TX_FEES     : `0 * COIN`

};
