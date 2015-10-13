#pragma once

#include <stdint.h>

#include "uint256.h"

class CBigNum; // We only to forward-declare it, since we use it for a typedef
               // Not including "bignum.h" avoids cyclic dependencies

typedef int64_t  money_t;
typedef int64_t  timestamp_t;
typedef int32_t  blockheight_t;
typedef uint256  hash_t;
typedef CBigNum  target_t;
typedef uint32_t compact_target_t;

static money_t const CENT = 10000;
static money_t const COIN = 100 * CENT;

static timestamp_t const SECOND = 1;
static timestamp_t const MINUTE = 60 * SECOND;
static timestamp_t const HOUR   = 60 * MINUTE;
static timestamp_t const DAY    = 24 * HOUR;
static timestamp_t const WEEK   = 7 * DAY;
static timestamp_t const MONTH  = 730 * HOUR;
static timestamp_t const YEAR   = 365 * DAY;

#include "bignum.h"
