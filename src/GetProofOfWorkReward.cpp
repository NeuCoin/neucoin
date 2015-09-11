#include "constants.h"
#include "types.h"

#include "GetProofOfWorkReward.h"

money_t GetProofOfWorkReward(blockheight_t prevHeight)
{
    if (prevHeight == 0 && COIN_PREMINE > 0)
        return COIN_PREMINE;

    if (prevHeight < POW_DUMMY_BLOCK_COUNT)
        return 1 * CENT;

    if (prevHeight >= POW_MAX_BLOCK)
        return 1 * CENT;

    return POW_BLOCK_REWARD;
}
