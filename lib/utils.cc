#include "utils.h"
namespace smpc {
  int64_t modPow (int64_t num, uint32_t exp, int64_t mod) {
    __int128_t result = 1;
    __int128_t n = num;
    for (result = 1; exp; exp >>= 1) {
      if (exp & 1) {
        result = ((result % mod) * (n % mod)) % mod;
      }
      n = ((n % mod) * (num % mod)) % mod;
    }
    return (int64_t)result;
  }
}
