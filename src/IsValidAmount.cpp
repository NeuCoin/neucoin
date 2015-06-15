#include "constants.h"
#include "types.h"

bool IsValidAmount(money_t amount)
{
    return amount >= 0 && amount < MAX_MONEY_STACK;
}
