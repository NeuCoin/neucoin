#pragma once

#include "constants.h"
#include "main.h"

#define UNSIGNED(N) static_cast<unsigned>(N)
#define IP_ADDRESS(A, B, C, D) ((UNSIGNED(D) << 24) | (UNSIGNED(C) << 16) | (UNSIGNED(B) << 8) | UNSIGNED(A))
