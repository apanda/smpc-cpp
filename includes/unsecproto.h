#include <cassert>
#include <stdint.h>
#include <cstdio>
#include "problem.h"
#pragma once

namespace smpc {
  bool unsecurePathComputaton (BGPProblem* problem, int32_t dest, uint32_t* nextHops, size_t nhopSize);
}
