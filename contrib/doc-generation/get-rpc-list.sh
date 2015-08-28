#!/usr/bin/env bash

DIRECTORY="$(dirname "$(readlink -f "$0")")"

cd "$DIRECTORY"/../../src
make -f Makefile.unix neucoind >&2

rm -rf /tmp/tmpcoindir
mkdir -p /tmp/tmpcoindir

./neucoind -testnet -rpcport=2001 -rpcuser=user -rpcpassword=password -port=2000 -rpcport=2001 -datadir=/tmp/tmpcoindir -dnsseed=0 >&2 &
NODE=$!

htmlentities() {
    sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g; s/'"'"'/\&#39;/g'
}

sleep 5

printf '# RPC List\n'
printf '\n'
printf '## Summary\n'
printf '\n'

printf '<table><tr>\n'

COL=0

while read COMMAND; do

    NAME="${COMMAND%% *}"
    PARAMETERS="${COMMAND#* }"

    if [[ $(($COL % 3)) -eq 0 && $COL -ne 0 ]]; then
        printf '</tr><tr>\n'
    fi

    COL=$(($COL + 1))

    printf '<td><a href="#%s">%s</a></td>\n' "$(printf "%s" "$NAME" | sed 's/[^a-z]\+/-/g' | sed 's/^-\+\|-\+$//g')" "$NAME"

done < <(./neucoind -rpcport=2001 -rpcuser=user -rpcpassword=password help)

printf '</tr></table>\n'

printf '\n'
printf '## Full details'
printf '\n'

while read COMMAND; do

    NAME="${COMMAND%% *}"
    PARAMETERS="${COMMAND#* }"

    HELP="$(./neucoind -rpcport=2001 -rpcuser=user -rpcpassword=password help "$NAME" | tail -n+2 | htmlentities)"

    printf '### %s\n' "$NAME"
    printf '\n'
    printf '```\n'
    printf '%s %s\n' "$NAME" "$PARAMETERS"
    printf '```\n'
    printf '\n'
    printf '%s\n' "$HELP"
    printf '\n'

done < <(./neucoind -rpcport=2001 -rpcuser=user -rpcpassword=password help)

kill -9 $NODE
