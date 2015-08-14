#!/usr/bin/env bash

DIRECTORY="$(dirname "$(readlink -f "$0")")"
EXITCODE=0

for FILE in "$DIRECTORY"/test-*.js; do

    TEST="$(basename "$FILE" .js)"

    echo Running "$TEST"...

    if ! node $DIRECTORY/_runner ./"$TEST"; then
        EXITCODE=1
    fi

done

exit "$EXITCODE"
