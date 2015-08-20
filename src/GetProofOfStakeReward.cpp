#include <stdint.h>

#include "constants.h"
#include "types.h"

#include "GetProofOfStakeReward.h"

money_t GetProofOfStakeReward(timestamp_t coinAge, blockheight_t height)
{
    if (POS_BLOCK_REWARD)
        return POS_BLOCK_REWARD;

    // The actual formula is shaped as (A + exp(B * nHeight) + C)
    // Since we can't use float, and that using fixed-point numbers would be both complex and error-prone, we chose to use a step-by-step solution
    // Each array cell is the reward given starting from a specific bloc height. Each cells will be applied for the next 43,836 blocks (about a month)
    // Note that these values have to be divided by 10,000 later (ie 9278 is actually 0.9278).

    static uint16_t const rewardSteps[] = { 10000, 9754, 9593, 9435, 9278, 9124, 8972, 8823, 8675, 8530, 8386, 8245, 8106, 7969, 7834, 7701, 7569, 7440, 7312, 7187, 7063, 6941, 6821, 6702, 6585, 6470, 6356, 6245, 6134, 6026, 5919, 5813, 5709, 5607, 5505, 5406, 5308, 5211, 5116, 5022, 4929, 4838, 4748, 4659, 4572, 4486, 4401, 4318, 4235, 4154, 4074, 3995, 3917, 3841, 3765, 3691, 3617, 3545, 3474, 3404, 3334, 3266, 3199, 3133, 3067, 3003, 2940, 2877, 2816, 2755, 2695, 2636, 2578, 2521, 2464, 2409, 2354, 2300, 2247, 2194, 2142, 2091, 2041, 1992, 1943, 1895, 1847, 1801, 1755, 1709, 1664, 1620, 1577, 1534, 1492, 1450, 1409, 1369, 1329, 1290, 1251, 1213, 1176, 1139, 1102, 1066, 1031, 996, 961, 928, 894, 861, 829, 797, 765, 734, 703, 673, 643, 600 };

    uint32_t stepIndex = height / 43836;
    uint32_t stepCount = sizeof( rewardSteps ) / sizeof( rewardSteps[ 0 ] );

    if ( stepIndex >= stepCount )
        stepIndex = stepCount - 1;

    money_t rewardCoinYear = COIN * rewardSteps[ stepIndex ] / 10000;

    // So basically, we're getting the number of coin per year (above), divided by the number of days in a year, multiplied by the coin age (in days) of the stake (prorata).
    // If the end of the formula looks weird to you, it's because there isn't exactly 365 days in a year - that would be forgetting the leap years ... So there's actually 365.2421 days.
    // "( 365 * 33 + 8 ) / 33" is approximatively 0.2424. "* 33 / ( 365 * 33 + 8 )" is a way to take this in account without risking losing precision (ie. without using floats).

    return coinAge * rewardCoinYear * 33 / ( 365 * 33 + 8 );
}
