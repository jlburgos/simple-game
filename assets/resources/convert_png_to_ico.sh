#!/bin/bash

[ -z "${1}" ] && {
    echo "Missing PNG file argument!"
    exit 1
}

[ -z "${2}" ] && {
    echo "Missing ICO file argument!"
    exit 1
}

convert -verbose \
        -resize x16 \
        -gravity center \
        -crop 16x16+0+0 \
        -flatten \
        -colors 256 \
        -background transparent \
        "${1}" "${2}"