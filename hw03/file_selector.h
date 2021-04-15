#pragma once
#include <linenoise.h>
#include <stdbool.h>

void file_completion(const char *buf, linenoiseCompletions *lc);

// The @param str must had prefix complete
bool strPreCmp(const char *str, const char *target);

// The @param str must had postfix complete
bool strPostCmp(const char *str, const char *target);
