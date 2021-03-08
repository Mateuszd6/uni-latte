#!/bin/sh

rm -f ./big-test.lat
touch ./big-test.lat
for i in {1..800}
do
    cat ./big-test-impl.lat | sed "s/001/$i/g" >> big-test.lat
done
echo "int main() { return 0; }" >> big-test.lat
