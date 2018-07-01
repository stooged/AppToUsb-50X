#!/bin/bash
set -e
pushd tool
make
popd
pushd AppToUsb
make
popd
mkdir -p bin
rm -f bin/AppToUsb.bin
cp AppToUsb/AppToUsb.bin bin/AppToUsb.bin
mkdir -p html_payload
tool/bin2js bin/AppToUsb.bin > html_payload/payload.js
FILESIZE=$(stat -c%s "bin/AppToUsb.bin")
PNAME=$"AppToUsb"
cp exploit.template html_payload/AppToUsb.html
sed -i -f - html_payload/AppToUsb.html << EOF
s/#NAME#/$PNAME/g
s/#BUF#/$FILESIZE/g
s/#PAY#/$(cat html_payload/payload.js)/g
EOF
rm -f AppToUsb/AppToUsb.bin
rm -f html_payload/payload.js
