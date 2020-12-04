#pragma once

#include "basic.h"
#include <stdlib.h>

fp mean(i64 *a, size_t size);
fp stddevSQ(i64 *, size_t);
fp covariance(i64 *a, i64 *b, size_t size);
