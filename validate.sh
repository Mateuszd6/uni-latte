#!/bin/sh

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
COV_IDX=0

rm -rf ./cov
rm -rf ./outs
mkdir -p ./cov
mkdir -p ./outs
echo -n "" > failed-tests.out

echo "Running \"good\" tests."
for f in $(ls ./tests/good/*.lat ./tests/good/**/*.lat 2> /dev/null); do
    KOVNAME="./cov/cov_${COV_IDX}"
    let "COV_IDX += 1";
    kcov $KOVNAME ./latc $f > /dev/null 2> temp.out
    ECODE=$?

    grep -qE 'UndefinedBehaviorSanitizer|AddressSanitizer|MemorySanitizer|Unreachable code reached' < temp.out
    SANITIZED=$?

    if [ "OK" == "`head -n1 temp.out`" ] && [ $ECODE -eq 0 ] && [ $SANITIZED -eq 1 ]; then
        let "PASSED_TESTS += 1";
        let "TOTAL_TESTS += 1";
    else
        echo $f >> failed-tests.out
        let "FAILED_TESTS += 1";
        let "TOTAL_TESTS += 1";
    fi
done

echo "Running \"bad\" tests."
for f in $(ls ./tests/bad/*.lat ./tests/bad/**/*.lat 2> /dev/null); do
    KOVNAME="./cov/cov_${COV_IDX}"
    let "COV_IDX += 1";
    kcov $KOVNAME ./latc $f > /dev/null 2> temp.out
    ECODE=$?

    grep -qE 'UndefinedBehaviorSanitizer|AddressSanitizer|MemorySanitizer|Unreachable code reached' < temp.out
    SANITIZED=$?

    if [ "ERROR" == "`head -n1 temp.out`" ] && [ $ECODE -ne 0 ] && [ $SANITIZED -eq 1 ]; then
        let "PASSED_TESTS += 1";
        let "TOTAL_TESTS += 1";
    else
        echo $f >> failed-tests.out
        let "FAILED_TESTS += 1";
        let "TOTAL_TESTS += 1";
    fi
done

echo "Running \"extension\" tests."
for f in $(ls ./tests/extensions/*.lat ./tests/extensions/**/*.lat 2> /dev/null); do
    KOVNAME="./cov/cov_${COV_IDX}"
    let "COV_IDX += 1";
    kcov $KOVNAME ./latc $f > /dev/null 2> temp.out
    ECODE=$?

    grep -qE 'UndefinedBehaviorSanitizer|AddressSanitizer|MemorySanitizer|Unreachable code reached' < temp.out
    SANITIZED=$?

    if [ "OK" == "`head -n1 temp.out`" ] && [ $ECODE -eq 0 ]  && [ $SANITIZED -eq 1 ]; then
        let "PASSED_TESTS += 1";
        let "TOTAL_TESTS += 1";
    else
        echo $f >> failed-tests.out
        let "FAILED_TESTS += 1";
        let "TOTAL_TESTS += 1";
    fi
done

kcov --merge ./cov ./cov/cov_*

echo -n "Passed $PASSED_TESTS/$TOTAL_TESTS tests. "

if [ $FAILED_TESTS -eq 0 ]; then
    echo "Good job!";
else
    echo "Failed tests:";
    cat ./failed-tests.out
fi

rm ./failed-tests.out
rm ./temp.out
