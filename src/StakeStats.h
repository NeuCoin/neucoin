#pragma once

#include "bignum.h"
#include "types.h"

class StakeStats
{

 public:

    StakeStats(void);

    void PushChancesToMint(CBigNum bnChancesToMint);

    timestamp_t GetEstimatedStakeTime(double expectedChancesToMint = 0.9);

 private:

    double m_ProbabilitiesNotToMint;

};
