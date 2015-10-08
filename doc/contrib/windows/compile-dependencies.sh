#!/usr/bin/env bash

DEPENDENCY_PATH="/c/Dependencies/"

# You should not have to edit this file starting from here

set -e
set -u

PATH=${DEPENDENCY_PATH}/mingw32/bin:$PATH

cd "${DEPENDENCY_PATH}"
WIN_DEPENDENCY_PATH="$(pwd -W)"

find -maxdepth 1 -name '*.gz' -o -name '*.tar' -o -name '*.tar.gz' | while read archive; do

  target="${archive}"
  target="${target%%-*}"
  target="${target%%_*}"

  if [ ! -d "${target}" ]; then
    echo "Extracting ${archive} to ${target}"
    mkdir -p "${target}"
    tar xf "${archive}" -C "${target}"
  fi

done

winpath() {
  printf "%s" "${1//\//\\\\}"
}

DB_PATH="$(ls -d ./db/db-*/)"
OPENSSL_PATH="$(ls -d ./openssl/openssl-*/)"
MINIUPNPC_PATH="$(ls -d ./miniupnpc/miniupnpc-*/)"
PROTOBUF_PATH="$(ls -d ./protobuf/protobuf-*/)"
LIBPNG_PATH="$(ls -d ./libpng/libpng-*/)"
QRENCODE_PATH="$(ls -d ./qrencode/qrencode-*/)"
BOOST_PATH="$(ls -d ./boost/boost_*/)"
QTBASE_PATH="$(ls -d ./qtbase-opensource-src-*/)"
QTTOOLS_PATH="$(ls -d ./qttools-opensource-src-*/)"

BUILD_OPENSSL=1
BUILD_DB=1
BUILD_MINIUPNPC=1
BUILD_PROTOBUF=1
BUILD_LIBPNG=1
BUILD_QRENCODE=1
BUILD_BOOST=1
BUILD_QTBASE=1
BUILD_QTTOOLS=1

if [[ ! -e "${MINIUPNPC_PATH}"/miniupnpc ]]; then
  # The bitcoin source code uses "include <miniupnpc/miniwget>", so we need to add a symlink to itself
  ln -s "${DEPENDENCY_PATH}"/"${MINIUPNPC_PATH}" "${MINIUPNPC_PATH}"/miniupnpc
fi

[[ ${BUILD_OPENSSL} = 1 ]] && ( cd "${OPENSSL_PATH}" &&
  ./Configure no-zlib no-shared no-dso no-krb5 no-camellia no-capieng no-cast no-cms no-dtls1 no-gost no-gmp no-heartbeats no-idea no-jpake no-md2 no-mdc2 no-rc5 no-rdrand no-rfc3779 no-rsax no-sctp no-seed no-sha0 no-static_engine no-whirlpool no-rc2 no-rc4 no-ssl2 no-ssl3 mingw &&
  make )

[[ ${BUILD_DB} = 1 ]] && ( cd "${DB_PATH}"/build_unix &&
  ../dist/configure --enable-mingw --enable-cxx --disable-shared --disable-replication &&
  make )

[[ ${BUILD_MINIUPNPC} = 1 ]] && ( cd "${MINIUPNPC_PATH}" &&
  mingw32-make -f Makefile.mingw init upnpc-static )

[[ ${BUILD_PROTOBUF} = 1 ]] && ( cd "${PROTOBUF_PATH}" &&
  configure --disable-shared &&
  make )

[[ ${BUILD_LIBPNG} = 1 ]] && ( cd "${LIBPNG_PATH}" &&
  configure --disable-shared &&
  make &&
  rm -vf .libs/libpng.a &&
  cp .libs/libpng*.a .libs/libpng.a )

[[ ${BUILD_QRENCODE} = 1 ]] && ( cd "${QRENCODE_PATH}" &&
  LIBS="$(ls "${DEPENDENCY_PATH}"/"${LIBPNG_PATH}"/.libs/libpng.a)" \
  png_CFLAGS="-I'${DEPENDENCY_PATH}'/'${LIBPNG_PATH}'" \
  png_LIBS="-L'${DEPENDENCY_PATH}'/'${LIBPNG_PATH}'/.libs" \
  configure --enable-static --disable-shared --without-tools &&
  make )

[[ ${BUILD_BOOST} = 1 ]] && ( cd "${BOOST_PATH}" && ( cmd <<'EOT'
  bootstrap.bat mingw
  b2 --build-type=complete --with-chrono --with-filesystem --with-program_options --with-system --with-thread toolset=gcc variant=release link=static threading=multi runtime-link=static stage
EOT
) && ( cd "stage/lib" && for lib in *-*.a; do
    cp "${lib}" "${lib%%-*}.${lib#*.}"
  done )
)

[[ ${BUILD_QTBASE} = 1 ]] && ( cd "${QTBASE_PATH}" && cmd <<EOT
  set INCLUDE=$(winpath "${WIN_DEPENDENCY_PATH}/${LIBPNG_PATH}");$(winpath "${WIN_DEPENDENCY_PATH}/${OPENSSL_PATH}/include")
  set LIB=$(winpath "${WIN_DEPENDENCY_PATH}/${LIBPNG_PATH}/.libs");$(winpath "${WIN_DEPENDENCY_PATH}/${OPENSSL_PATH}/include")

  configure.bat -release -opensource -confirm-license -static -make libs -no-sql-sqlite -no-opengl -system-zlib -qt-pcre -no-icu -no-gif -system-libpng -no-libjpeg -no-freetype -no-angle -openssl -no-dbus -no-audio-backend -no-wmf-backend -no-qml-debug

  mingw32-make
EOT
)

[[ ${BUILD_QTTOOLS} = 1 ]] && ( cd "${QTTOOLS_PATH}" && (
  if ! grep '^#include <cstdio>$' src/windeployqt/main.cpp > /dev/null; then
    (echo '#include <cstdio>'; cat src/windeployqt/main.cpp) > tmp
    mv tmp src/windeployqt/main.cpp
  fi
) && cmd <<EOT
  set INCLUDE=$(winpath "${WIN_DEPENDENCY_PATH}/${LIBPNG_PATH}");$(winpath "${WIN_DEPENDENCY_PATH}/${OPENSSL_PATH}/include")
  set LIB=$(winpath "${WIN_DEPENDENCY_PATH}/${LIBPNG_PATH}/.libs");$(winpath "${WIN_DEPENDENCY_PATH}/${OPENSSL_PATH}/include")

  set PATH=%PATH%;$(winpath "${WIN_DEPENDENCY_PATH}/${QTBASE_PATH}/bin")

  qmake qttools.pro
  mingw32-make
EOT
)
