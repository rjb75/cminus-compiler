#!/bin/bash

echo "Running analyzer test cases"
COUNT=0
FAILED=0

rm analyzer-test-err.log

for f in ./tests/analyzer/*.cm; do
    [ -e "$f" ] || continue
    COUNT=$((COUNT + 1))
    name=${f##*/}
    echo $name >> analyzer-test-err.log
    rm "${f}.tmp.err"
    ./analyzer -o "${f}.tst.res" "$f" 2>"${f}.tmp.err"
    if [ -f  "${f}.tst.res" ]
    then
        diff "${f}.res" "${f}.tst.res"
        if [ $? -eq 0 ]
        then
            echo "✅ Success for ${name}"
        else
            echo "❌ Fail for ${name}"
            FAILED=$((FAILED + 1)) 
        fi
    else
        diff "${f}.err" "${f}.tmp.err"
        if [ $? -eq 0 ]
        then
            echo "✅ Success for ${name}"
        else
            echo "❌ Fail for ${name}"
            FAILED=$((FAILED + 1)) 
        fi
    fi
done

echo -e "\033[0m"

echo "Ran ${COUNT} tests"

if [ $FAILED -eq 0 ]
then
    echo "${COUNT}/${COUNT} test(s) passed!"
    exit 0
else
    echo "${FAILED} test(s) failed!"
    exit -1
fi
