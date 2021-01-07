#!/bin/sh

echo -n "" > stress-test.lat
for i in {1..10000}
do
    cat ./tests/good/core001.lat \
        | sed -e "s/main/main_${i}/g" \
              -e "s/ifac2f/ifac2f_${i}/g" \
              -e "s/fac/fac_${i}/g" \
              -e "s/repStr/repStr_${i}/g" \
              >> stress-test.lat
    echo "" >> stress-test.lat
    echo "" >> stress-test.lat
done

echo "int main() { return 0; }" >> stress-test.lat
