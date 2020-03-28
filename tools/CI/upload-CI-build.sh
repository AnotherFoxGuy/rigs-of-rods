#!/bin/sh

set -eu


# Copy some libs
cp /usr/lib/x86_64-linux-gnu/libzzip-0.so.13            ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libfreeimage.so.3          ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libjpegxr.so.0             ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libjxrglue.so.0            ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libraw.so.15               ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libwebp.so.5               ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libwebpmux.so.1            ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libIlmImf-2_2.so.22        ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libjasper.so.1             ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libHalf.so.12              ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libIex-2_2.so.12           ./redist/lib/
cp /lib/x86_64-linux-gnu/libpng12.so.0                  ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libIlmThread-2_2.so.12     ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libCg.so                   ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libjpeg.so.8               ./redist/lib/
cp /usr/lib/x86_64-linux-gnu/libXaw.so.7                ./redist/lib/

ninja zip_and_copy_resources
ninja install
7z a ror-release ./redist/* 

