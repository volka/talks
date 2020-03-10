#!/bin/bash

INPUT_FOLDER=${INPUT_FOLDER:-`pwd`}
OUTPUT_FOLDER=${OUTPUT_FOLDER:-$INPUT_FOLDER/dist}
THEME=${1:-black}
TRANSITION=${2:-none}

# run once:
mkdir -p $OUTPUT_FOLDER
for f in *.puml; do plantuml -tsvg $f -o $OUTPUT_FOLDER; done
pandoc -t revealjs -s -o ${OUTPUT_FOLDER}/Rust_Intro.html Rust_Intro.md  \
    	-V theme=${THEME} -V transition=${TRANSITION} \
    	-V revealjs-url=template -Vprogress=true -Vwidth=\'90%\' -Vheight=\'90%\' \
    	-V slideNumber=true -V history=true \
    	--standalone --slide-level 2 --resource-path=${IMAGES_FOLDER}
mkdir -p $OUTPUT_FOLDER/template
cp *.png $OUTPUT_FOLDER
cp *.svg $OUTPUT_FOLDER
cp *.jpg $OUTPUT_FOLDER
cp -r reveal.js/lib $OUTPUT_FOLDER/template/
cp -r reveal.js/js $OUTPUT_FOLDER/template/
cp -r reveal.js/css $OUTPUT_FOLDER/template/
