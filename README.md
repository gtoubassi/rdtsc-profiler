# RDTSC Cycle Profiler

`cycle-profile.[ch]` implements a very simple cpu cycle profiler based
on the x86 `rdtsc` instruction.  See `main.c` for an example usage which compares
parsing longs with `sscanf` vs `strtol`.

# How to build

    $ clang -o main main.c cycle-profiler.c

# How to run

    $ ./main

# How to see results

    $ cat ./profile.out
    START 0 14838473262650 sscanf
    END 2464075391 14840937338041 sscanf
    START 0 14840937339133 strtol
    END 846938726 14841784277859 strtol

This shows that it took 2464075391 / 846938726 = 2.9x as long to parse with `sscanf` vs `strtol`.
This makes sense given `sscanf` needs to parse a format string.

You can use awk to sum across all measurements with the same label (which admittedly is uninteresting
in this case)

    $ cat profile.out | grep END | awk '$1 == "END" {totals[$4] += $2} END {for (v in totals) print v,totals[v]}'
    strtol 846938726
    sscanf 2464075391



