#ifndef QEMU_6_1_0_CYCLE_PROFILER_H
#define QEMU_6_1_0_CYCLE_PROFILER_H

void CycleProfileStart(const char *label);
void CycleProfileEnd(const char *label);
void CycleProfileWrite(const char *path);

#endif //QEMU_6_1_0_CYCLE_PROFILER_H
