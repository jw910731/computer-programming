#pragma once
#include "basic.h"

// Number Generator for game
i32 number_gen(i32);

// response generator
i32 guess(i32 ans, i32 guess, i32 n);

// Helper function for 
i32 cnt(i32 a);

// check if number digits are unique
bool unique_ck(i32 n, i32 size);