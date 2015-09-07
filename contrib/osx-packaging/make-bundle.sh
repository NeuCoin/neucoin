#!/usr/bin/env bash

DIRECTORY="$(cd "$(dirname "$0")"; pwd)"

BUNDLE="$1"
BINARY="$2"

rm -rf "$BUNDLE"

mkdir -p "$BUNDLE"/
mkdir -p "$BUNDLE"/Contents/
mkdir -p "$BUNDLE"/Contents/Libraries/
mkdir -p "$BUNDLE"/Contents/MacOS/
mkdir -p "$BUNDLE"/Contents/Plugins/
mkdir -p "$BUNDLE"/Contents/Plugins/platforms/
mkdir -p "$BUNDLE"/Contents/Resources/

push_dependencies()
{
    echo Processing "$1"...

    DEPENDENCIES="$(otool -L "$1" | grep -o -E '^\t/usr/local/[^ ]+' | sed 's/^.//g')"

    for DEPENDENCY in $DEPENDENCIES; do
        NAME=$(basename "$DEPENDENCY")

	echo Installing "$NAME" "($DEPENDENCY)"
        install_name_tool -change "$DEPENDENCY" @executable_path/../Libraries/"$NAME" "$1"

        if [[ ! -f "$BUNDLE"/Contents/Libraries/"$NAME" ]]; then
            cp "$DEPENDENCY" "$BUNDLE"/Contents/Libraries/ && chmod 644 "$BUNDLE"/Contents/Libraries/"$NAME"
            push_dependencies "$BUNDLE"/Contents/Libraries/"$NAME"
        fi
    done
}

cp "$DIRECTORY"/Info.plist "$BUNDLE"/Contents/
cp "$DIRECTORY"/Icon.icns "$BUNDLE"/Contents/Resources/

cp "$BINARY" "$BUNDLE"/Contents/MacOS/client-qt
push_dependencies "$BUNDLE"/Contents/MacOS/client-qt

cp /usr/local/opt/qt5/plugins/platforms/libqcocoa.dylib "$BUNDLE"/Contents/Plugins/platforms/
push_dependencies "$BUNDLE"/Contents/Plugins/platforms/libqcocoa.dylib
