#!/bin/bash

PIXMAP_FOLDER="pixmaps"
TMP_FILE=`mktemp --suffix=.svg`

svg_stroke=(
 "16#14"
 "20#14"
 "32#10"
 "48#6"
 "64#4"
 "80#4"
"128#4"
"256#4")

if [ $# -eq 1 ]; then
  FILE="$1"
  if [ ! -f "$FILE" ]; then
    echo "$FILE is non-existent"
    exit 1
  fi
else
  echo "Usage: $0 <filename> <dir>"
  exit 1
fi

update_stroke()
{
  sed 's#stroke-width:.*;#stroke-width:'"$1"';#' < "$FILE" > "$TMP_FILE"
}

check_bin()
{
  local bin_path=`which "$1" 2>/dev/null`
  if [ -x "$bin_path" ]; then
    local bin_name=`echo -n "$1" | tr '[:lower:]' '[:upper:]'`
    export "${bin_name}=${bin_path}"
  fi
}

check_bin convert
check_bin png2icns

if [ x"$CONVERT" == "x" ]; then
  echo "ImageMagick not present"
  exit 1
fi
[ x"$PNG2ICNS" == "x" ] && echo "icnsutils not available no icns file will be generated"

mkdir -p "$PIXMAP_FOLDER"

for i in ${svg_stroke[@]}; do
  PIX_SIZE=`echo -n $i | cut -d'#' -f1`
  LINE_STROKE=`echo -n $i | cut -d'#' -f2`
  update_stroke $LINE_STROKE
  $CONVERT -geometry "$PIX_SIZE" -background none "$TMP_FILE" \
                    "${PIXMAP_FOLDER}/neucoin${PIX_SIZE}.png"
  $CONVERT -geometry "$PIX_SIZE" -background none "$TMP_FILE" \
                    "${PIXMAP_FOLDER}/neucoin${PIX_SIZE}.bmp"
  $CONVERT -geometry "$PIX_SIZE" -background none "$TMP_FILE" \
                    "${PIXMAP_FOLDER}/neucoin${PIX_SIZE}.xpm"
  ico_list_bmp="${ico_file_list} ${PIXMAP_FOLDER}/neucoin${PIX_SIZE}.bmp"
  ico_list_png="${ico_file_list} ${PIXMAP_FOLDER}/neucoin${PIX_SIZE}.png"
done

$CONVERT ${ico_list_bmp} "${PIXMAP_FOLDER}/neucoin.ico"   # png ico is only Windows7+
[ x"$PNG2ICNS" != "x" ] && $PNG2ICNS "${PIXMAP_FOLDER}/neucoin.icns" ${ico_list_png}

rm -f "$TMP_FILE"
