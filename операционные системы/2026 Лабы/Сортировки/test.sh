#!/bin/bash

PROGRAM=./third
OUTPUT=results.txt
RUNS=3

# размеры массивов (2^I)
SIZES=(18 19 20 21)

# глубины
DEPTHS=(0 1 2 3 4 5)

echo "I,n,depth,avg_time_ms" > $OUTPUT

for I in "${SIZES[@]}"
do
    N=$((1 << I))

    for depth in "${DEPTHS[@]}"
    do
        echo "Running I=$I (n=$N), depth=$depth..."

        SUM=0

        for ((run=0; run<$RUNS; run++))
        do
            START=$(date +%s%N)

            $PROGRAM $depth $I > /dev/null

            END=$(date +%s%N)

            TIME_MS=$(( (END - START) / 1000000 ))
            SUM=$((SUM + TIME_MS))
        done

        AVG=$((SUM / RUNS))

        echo "$I,$N,$depth,$AVG" >> $OUTPUT
    done
done

echo "Done! Results saved to $OUTPUT"