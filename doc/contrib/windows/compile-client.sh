#!/usr/bin/env bash

DEPENDENCY_PATH="/c/Dependencies/"

# You should not have to edit this file starting from here

set -e
set -u

PATH=${DEPENDENCY_PATH}/mingw32/bin:$PATH

make -f makefile.mingw \
  \
  MOC="$(ls -d ${DEPENDENCY_PATH}/qtbase-opensource-src-*/bin/moc.exe)"\
  UIC="$(ls -d ${DEPENDENCY_PATH}/qtbase-opensource-src-*/bin/uic.exe)"\
  RCC="$(ls -d ${DEPENDENCY_PATH}/qtbase-opensource-src-*/bin/rcc.exe)"\
  LRELEASE="$(ls -d ${DEPENDENCY_PATH}/qtbase-opensource-src-*/bin/lrelease.exe)"\
  \
  DB_PATH="$(ls -d ${DEPENDENCY_PATH}/db/db-*/)" \
  OPENSSL_PATH="$(ls -d ${DEPENDENCY_PATH}/openssl/openssl-*/)" \
  MINIUPNPC_PATH="$(ls -d ${DEPENDENCY_PATH}/miniupnpc/miniupnpc-*/)" \
  PROTOBUF_PATH="$(ls -d ${DEPENDENCY_PATH}/protobuf/protobuf-*/)" \
  LIBPNG_PATH="$(ls -d ${DEPENDENCY_PATH}/libpng/libpng-*/)" \
  QRENCODE_PATH="$(ls -d ${DEPENDENCY_PATH}/qrencode/qrencode-*/)" \
  BOOST_PATH="$(ls -d ${DEPENDENCY_PATH}/boost/boost_*/)" \
  QTBASE_PATH="$(ls -d ${DEPENDENCY_PATH}/qtbase-opensource-src-*/)" \
  QTTOOLS_PATH="$(ls -d ${DEPENDENCY_PATH}/qttools-opensource-src-*/)" \
  "$@"
