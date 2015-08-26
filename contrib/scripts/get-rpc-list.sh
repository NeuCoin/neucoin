#!/usr/bin/env bash

DIRECTORY="$(dirname "$(readlink -f "$0")")"

cd "$DIRECTORY"/../../src
make -f Makefile.unix neucoind

rm -rf /tmp/tmpcoindir
mkdir -p /tmp/tmpcoindir

./neucoind -testnet -rpcport=2001 -rpcuser=user -rpcpassword=password -port=2000 -rpcport=2001 -datadir=/tmp/tmpcoindir -dnsseed=0 &
NODE=$!

htmlentities() {
    sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g; s/'"'"'/\&#39;/g'
}

sleep 5

./neucoind -rpcport=2001 -rpcuser=user -rpcpassword=password help | while read COMMAND; do

    NAME="${COMMAND%% *}"
    PARAMETERS="${COMMAND#* }"

    HELP="$(./neucoind -rpcport=2001 -rpcuser=user -rpcpassword=password help "$NAME" | tail -n+2 | htmlentities)"

    printf '  - \n'
    printf '    ```\n'
    printf '    %s\n' "$NAME $PARAMETERS"
    printf '    ```\n'
    printf '    \n'
    printf '%s\n' "$(sed 's/^/    /g' <<< "$HELP")"
    printf '\n'

done

kill -9 $NODE
