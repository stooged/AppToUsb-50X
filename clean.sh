#!/bin/bash
pushd tool
make clean
popd
pushd AppToUsb
make clean
popd
rm -f js_payload/payload.js
rm -f bin/AppToUsb.bin

