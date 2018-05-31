#!/bin/bash
pushd tool
make clean
popd
pushd AppToUsb
make clean
popd
rm -f html_payload/AppToUsb.html
rm -f bin/AppToUsb.bin

