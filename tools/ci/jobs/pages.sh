#!/bin/bash

export LOGFILE=pages.log

source ./tools/ci/scripts/init.sh

tools/ci/scripts/retry.sh git clone https://gitlab.com/4144/pagesindexgen.git pagesindexgen

mkdir -p ../public/docs
mkdir -p ../public/stats
mkdir -p ../public/gcov
mkdir -p ../public/version

cp logs/gcc-?.* ../public/gcov
cp logs/gcc-?_SDL2.* ../public/gcov
echo "Url: $CI_PROJECT_URL" >../public/version/commit.txt
echo "Ref name: $CI_BUILD_REF_NAME" >>../public/version/commit.txt
echo "Ref: $CI_BUILD_REF" >>../public/version/commit.txt

cd pagesindexgen
./pagesindexgen.py ../../public
check_error $?

cd ..
cp -r doxygen/html/* ../public/docs
cp -r stats/* ../public/stats
