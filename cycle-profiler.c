#include "cycle-profiler.h"
#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>

#define kTimingsPerBlock (1024*1024-1)

typedef struct CycleTimingsStruct {
    struct CycleTimingsStruct *next;
    struct {
        unsigned int end:1;
        unsigned long cycleCount:63;
        const char *label;
    } timings[kTimingsPerBlock];
} CycleTimings;

static CycleTimings *head;
static CycleTimings *curr;
static int currIndex;

static inline void ensureCapacity(void) {
    if (!head) {
        head = calloc(1, sizeof(CycleTimings));
        curr = head;
    }
    if (currIndex >= kTimingsPerBlock) {
        curr->next = calloc(1, sizeof(CycleTimings));
        curr = curr->next;
        currIndex = 0;
    }
}

void CycleProfileStart(const char *label) {
    unsigned int dummy;
    ensureCapacity();
    curr->timings[currIndex].label = label;
    curr->timings[currIndex].cycleCount = __rdtscp(&dummy);
    currIndex++;
}

void CycleProfileEnd(const char *label) {
    unsigned int dummy;
    unsigned long cycles = __rdtscp(&dummy);
    ensureCapacity();
    curr->timings[currIndex].end = 1;
    curr->timings[currIndex].cycleCount = cycles;
    curr->timings[currIndex].label = label;
    currIndex++;
}

void CycleProfileWrite(const char *path) {
    FILE *fp;
    if ((fp = fopen(path, "w"))) {
        for (CycleTimings *p = head; p; p = p->next) {
            for (int i = 0; i < kTimingsPerBlock && p->timings[i].label; i++) {
                unsigned long duration = 0;
                if (p->timings[i].end) {
                    // This won't work if the start/end calls cross a timing heap node (i.e.
                    // the start call is stored in a previous node in the linked list)
                    for (int j = i - 1; j >= 0; j--) {
                        if (p->timings[j].label == p->timings[i].label) {
                            duration = p->timings[i].cycleCount - p->timings[j].cycleCount;
                            break;
                        }
                    }
                }
                fprintf(fp, "%s %ld %ld %s\n", p->timings[i].end ? "END" : "START", duration, p->timings[i].cycleCount, p->timings[i].label);
            }
        }
        fclose(fp);
    }
}
