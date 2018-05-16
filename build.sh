#!/bin/bash
set -e
pushd tool
make
popd
pushd AppToUsb
make
popd
rm -f bin/AppToUsb.bin
cp AppToUsb/AppToUsb.bin bin/AppToUsb.bin
tool/bin2js bin/AppToUsb.bin > js_payload/payload.js
rm -f AppToUsb/AppToUsb.bin

