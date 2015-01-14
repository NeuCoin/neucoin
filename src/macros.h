#pragma once

#include "constants.h"
#include "main.h"

#define DYN_POW_TAIL GetLastBlockIndex(pindexBest, false)
#define DYN_POS_TAIL GetLastBlockIndex(pindexBest, true)

#define DYN_POW_HEIGHT DYN_POW_TAIL->nHeight
#define DYN_POS_HEIGHT DYN_POS_TAIL->nHeight

#define UNSIGNED(N) static_cast<unsigned>(N)
#define IP_ADDRESS(A, B, C, D) ((UNSIGNED(D) << 24) | (UNSIGNED(C) << 16) | (UNSIGNED(B) << 8) | UNSIGNED(A))
