#include <inttypes.h>

#include "constants.h"
#include "main.h"
#include "types.h"

#include "GetNextTargetRequired.h"

compact_target_t GetNextTargetRequired(CBlockIndex const * pindexTail0, bool fProofOfStake)
{
    target_t initialTarget = POW_INITIAL_TARGET;

    if (pindexTail0 == NULL)
        return initialTarget.GetCompact(); // genesis block

    CBlockIndex const * pindexTail1 = GetLastBlockIndex(pindexTail0, fProofOfStake);

    if (pindexTail1->pprev == NULL)
        return initialTarget.GetCompact(); // first block

    CBlockIndex const * pindexTail2 = GetLastBlockIndex(pindexTail1->pprev, fProofOfStake);

    if (pindexTail2->pprev == NULL)
        return initialTarget.GetCompact(); // second block

    timestamp_t actualSpacing = pindexTail1->GetBlockTime() - pindexTail2->GetBlockTime();
    timestamp_t targetSpacing = fProofOfStake ? POS_TARGET_SPACING : POW_TARGET_SPACING;
    timestamp_t targetInterval = TARGET_TIMESPAN / targetSpacing;

    // ppcoin: target change every block
    // ppcoin: retarget with exponential moving toward target spacing

    target_t currentTarget = CBigNum().SetCompact(pindexTail1->nBits);
    target_t newTarget = currentTarget;

    newTarget *= ((targetInterval - 1) * targetSpacing + actualSpacing + actualSpacing);
    newTarget /= ((targetInterval + 1) * targetSpacing);

    target_t maxTarget = fProofOfStake ? POS_MAX_TARGET : POW_MAX_TARGET;

    if (newTarget > maxTarget)
        newTarget = maxTarget;

    //printf("Expecting: %llis Actual: %llis Old Difficulty: %x New Difficulty: %x (%s)\n", targetSpacing, actualSpacing, currentTarget.GetCompact(), newTarget.GetCompact(), fProofOfStake ? "PoS" : "PoW");

    return newTarget.GetCompact();
}
