#!/usr/bin/env bash

if test ! -f thirdparty/build_v8.sh ;
then
  echo "ERROR: Please run this script from the root folder of v8capi"
  exit 1
fi

ROOT_DIR=$(pwd)

cd thirdparty

if test ! -d v8 ;
then
    rm -rf .cipd .gclient .gclient_entries
    ./depot_tools/fetch v8 || exit 1
fi

cd v8

git checkout 8.1.248 || exit 1
$ROOT_DIR/thirdparty/depot_tools/gclient sync || exit 1

if test $(uname) = Linux ;
then
    ./build/install-build-deps.sh
fi

tools/dev/v8gen.py x64.release.sample || exit 1
ninja -C out.gn/x64.release.sample v8_monolith || exit 1

if test ! -L libv8_monolith.a ;
then
    ln -s out.gn/x64.release.sample/obj/libv8_monolith.a . || exit 1
fi

cd $ROOT_DIR

exit 0
