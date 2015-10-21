#include <algorithm>

#include <boost/math/special_functions/fpclassify.hpp>

#include "StakeStats.h"
#include "bignum.h"
#include "types.h"

StakeStats::StakeStats(void)
{
    m_ProbabilitiesNotToMint = 1;
}

void StakeStats::PushChancesToMint(CBigNum bnChancesToMint)
{
    CBigNum bnMaxValue = CBigNum(~uint256(0));

    if (bnChancesToMint > bnMaxValue)
        bnChancesToMint = bnMaxValue;

    uint64_t precision = 1000000000000000000;

    CBigNum bnExtendedChancesToMint = bnChancesToMint * precision;
    CBigNum bnExtendedProbabilitiesToMint = bnExtendedChancesToMint / bnMaxValue;

    double probabilitiesToMint = static_cast<double>(bnExtendedProbabilitiesToMint.getuint64()) / precision;
    double probabilitiesNotToMint = 1 - probabilitiesToMint;

    m_ProbabilitiesNotToMint *= probabilitiesNotToMint;

    /* Just add a '/' before the '/*' to uncomment this whole block
    //
    printf("---\n");
    printf("bnChancesToMint               : %-100s\n", bnChancesToMint.ToString().c_str());
    printf("bnMaxValue                    : %-100s\n", bnMaxValue.ToString().c_str());
    printf("bnExtendedChancesToMint       : %-100s\n", bnExtendedChancesToMint.ToString().c_str());
    printf("bnExtendedProbabilitiesToMint : %-100s\n", bnExtendedProbabilitiesToMint.ToString().c_str());
    printf("extendedProbabilitiesToMint   : %.17g\n", static_cast<double>(bnExtendedProbabilitiesToMint.getuint64()));
    printf("probabilitiesToMint           : %.17g\n", probabilitiesToMint);
    printf("probabilitiesNotToMint        : %.17g\n", probabilitiesNotToMint);
    printf("---\n");
    //*/
}

timestamp_t StakeStats::GetEstimatedStakeTime(double expectedChancesToMint)
{
    double expectedChancesNotToMint = std::max(0., 1. - expectedChancesToMint);
    double expectedEventCount = log(expectedChancesNotToMint) / log(m_ProbabilitiesNotToMint);

    if (boost::math::isinf(expectedEventCount))
        return -1;

    return static_cast<timestamp_t>(expectedEventCount);
}
