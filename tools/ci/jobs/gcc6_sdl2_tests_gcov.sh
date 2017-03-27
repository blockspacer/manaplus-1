#!/bin/bash

export CC=gcc-6
export CXX=g++-6

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc6.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++1z -Wformat=1 \
-Wno-attributes"

source ./tools/ci/flags/gcc6.sh

do_init
run_configure --enable-unittests=yes --with-sdl2 --with-gcov $*
export SDL_VIDEODRIVER=dummy
run_make_check
run_gcov gcov-6 gcc-6_SDL2

source ./tools/ci/scripts/exit.sh

exit 0
