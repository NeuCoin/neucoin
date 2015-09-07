#!/usr/bin/env bash

DIRECTORY="$(cd "$(dirname "$0")"; pwd)"

if [[ ! $2 == /* ]]; then
    APP="$PWD"/"$2"
else
    APP="$2"
fi 

cat > "$DIRECTORY"/appdmg.json <<HEREDOC
{
    "title" : "Neucoin",
    "icon" : "Icon.icns",
    "background" : "Background.png",
    "icon-size" : 80,
    "contents" : [
        { "x" : 490, "y" : 360, "type" : "link", "path" : "/Applications" },
        { "x" : 210, "y" : 360, "type" : "file", "path" : "$APP" }
    ]
}
HEREDOC

rm -f "$1"
appdmg "$DIRECTORY"/appdmg.json "$1"
