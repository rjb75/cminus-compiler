#!/bin/bash

echo "Running scanner test cases"
COUNT=0
FAILED=0

rm scanner-test-err.log

for f in ./tests/scanner/*.cm; do
    [ -e "$f" ] || continue
    COUNT=$((COUNT + 1))
    name=${f##*/}
    ./scanner -o "${f}.tst.res" "$f" 2>>scanner-test-err.log
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
    echo "${FAILED} test(s) failed! See errors in scanner-test-err.log"
    exit -1
fi
