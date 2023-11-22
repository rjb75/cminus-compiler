#!/bin/bash

echo "Running parser test cases"
COUNT=0
FAILED=0

rm parser-test-err.log

for f in ./tests/parser/*.cm; do
    [ -e "$f" ] || continue
    COUNT=$((COUNT + 1))
    name=${f##*/}
    echo $name >> parser-test-err.log
    ./parser -o "${f}.tst.res" "$f" 2>>parser-test-err.log
    diff "${f}.res" "${f}.tst.res"
    if [ $? -eq 0 ]
    then
        echo "✅ Success for ${name}"
    else
        echo "❌ Fail for ${name}"
        FAILED=$((FAILED + 1)) 
    fi
done

echo "Ran ${COUNT} tests"

if [ $FAILED -eq 0 ]
then
    echo "${COUNT}/${COUNT} test(s) passed!"
    exit 0
else
    echo "${FAILED} test(s) failed! See errors in parser-test-err.log"
    exit -1
fi
