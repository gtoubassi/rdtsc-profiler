#include <stdio.h>
#include <stdlib.h>
#include "cycle-profiler.h"

#define kNumIterations 10000000

int main(int argc, const char **argv) {
    const char *input = "12345678910";
    long value;
    
    CycleProfileStart("sscanf");
    for (int i = 0; i < kNumIterations; i++) {
        sscanf(input, "%ld", &value);
    }
    CycleProfileEnd("sscanf");

    CycleProfileStart("strtol");
    for (int i = 0; i < kNumIterations; i++) {
        value = strtol(input, NULL, 10);
    }
    CycleProfileEnd("strtol");

    CycleProfileWrite("profile.out");
    return 0;
}

