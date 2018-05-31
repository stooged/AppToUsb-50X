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
tool/bin2js bin/AppToUsb.bin > html_payload/payload.js
sed "s/###/$(cat html_payload/payload.js)/" exploit.template > html_payload/AppToUsb.html
rm -f AppToUsb/AppToUsb.bin
rm -f html_payload/payload.js
