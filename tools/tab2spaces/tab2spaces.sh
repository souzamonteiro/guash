#!/bin/sh

EXEFILE_PATH=$1/tab2spaces

for f in `ls $2`; do
    exec "$EXEFILE_PATH" "$f";
done

