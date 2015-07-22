#include "constants.h"
#include "kernel.h"
#include "util.h"

#include "CheckClientSanity.h"

bool CheckClientSanity(bool fAllowVulnerabilities)
{
    if (COINBASE_MATURITY < 1)
        return error("[Sanity fail] The coinbase maturity should not be lower than 1, for technical reasons");

    if (STAKE_MIN_AGE < GetStakeModifierSelectionInterval() && !fAllowVulnerabilities)
        return error("[Sanity fail] The stake min age should not be lower than the stake modifier selection interval (%lu seconds). Otherwise, you would be vulnerable to some attacks.", GetStakeModifierSelectionInterval());

    if (STAKE_AGE_STEP < STAKE_MAX_AGE - STAKE_MIN_AGE)
        return error("[Sanity fail] The stake age step should not be lower than the intervale between the stake min age and the stake max age. Otherwise, minting will be impossible.");

    if (TARGET_TIMESPAN < POW_TARGET_SPACING)
        return error("[Sanity fail] The target timespan should not be lower than the proof-of-work target spacing");

    if (TARGET_TIMESPAN < POS_TARGET_SPACING)
        return error("[Sanity fail] The target timespan should not be lower than the proof-of-stake target spacing");

    return true;
}
