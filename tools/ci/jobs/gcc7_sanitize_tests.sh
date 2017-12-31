#!/bin/bash

export CC=gcc-7
export CXX=g++-7

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc7.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O2 -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-fno-omit-frame-pointer \
-fsanitize=address -fsanitize=undefined \
-fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=unreachable \
-fsanitize=vla-bound -fsanitize=null -fsanitize=return \
-fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=alignment \
-fsanitize=object-size -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow \
-fsanitize=nonnull-attribute -fsanitize=returns-nonnull-attribute -fsanitize=bool \
-fsanitize=enum -fsanitize=vptr -fsanitize=bounds-strict \
-fsanitize=leak \
-fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=bounds-strict \
-fsanitize-address-use-after-scope \
-std=gnu++1z -Wformat=1 \
-Wno-attributes"

#source ./tools/ci/flags/gcc7.sh

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure --enable-unittests=yes $*
export SDL_VIDEODRIVER=dummy
export ASAN_OPTIONS=detect_leaks=0:detect_stack_use_after_return=true:strict_init_order=true:detect_invalid_pointer_pairs=1
export LSAN_OPTIONS=suppressions=$(pwd)/tools/ci/scripts/lsansuppression_tests.txt
run_make_check

source ./tools/ci/scripts/exit.sh

exit 0
