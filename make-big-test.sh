#!/bin/bash

array=(600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
for j in ${array[*]}; do
    du -hk "big-test${j}.lat"
done
exit 0

array=(600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
for j in ${array[*]}; do
    echo $j
    rm -f ./big-test.lat
    touch ./big-test.lat
    for i in $(seq 1 $j)
    do
        cat ./big-test-impl.lat | sed "s/001/$i/g" >> big-test.lat
    done
    echo "int main() { return 0; }" >> big-test.lat
    mv big-test.lat "big-test${j}.lat"
done
