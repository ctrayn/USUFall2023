Calvin Passmore

A02107892

ECE 5460

# Homework 4 Simulated Annealing

To compile:

    $ make

To run after compiling

    $ make run

## Notes

After running the starting input I got the output

    E0: 12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlV
    Initial Cost: 287.521
    Error: Chip ratio not valid : 76.3391, please adjust starting parameters and rerun
    Ef: d2H3V4iV6V7H8VV9VaVbVcV1VeVfVgV5VjVkVlV
    Final Cost: 272.264

Since the chip ratio is only checked at the end, the program has no way of annealing towards a good chip ratio.

The final cost was 272.264, which is 15.257 better than the original.

If the constants were changed from the given, the score could be improved.
Changing the constants to:

    ratio : 0.99
    iseed : 5

Does make it take a little longer, but it results in a better chip ratio and a better final score of 265.

    E0: 12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlV
    Initial Cost: 287.521
    Error: Chip ratio not valid : 30.8454, please adjust starting parameters and rerun
    Ef: 92V345V6VHH7V8dHabHVcVV1VeVfVgViVjVkVlV
    Final Cost: 265

The theoretical minimum score, calculated by summing all areas, is 195.15.
Considering that this score is literally impossible, 265 is about 35% error, or about 26% dead space.

The next initial states were similar