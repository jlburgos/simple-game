#!/bin/bash

[ -z "${1}" ] && {
    echo "Missing PNG file argument!"
    exit 1
}

convert -verbose \
        -resize x16 \
        -gravity center \
        -crop 16x16+0+0 \
        -flatten \
        -colors 256 \
        -background transparent \
        "${1}" favicon.ico