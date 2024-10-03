#!/bin/sh

MODS_DIR="./mods"
LIBS_DIR="../build/lib/modules/modules"

rm $MODS_DIR/*

for mod in $(ls $LIBS_DIR); do
    mod_dir=$LIBS_DIR/$mod
    if [[ -d $LIBS_DIR/$mod ]]; then
        pushd $MODS_DIR
        for file in $(ls ../$mod_dir); do
            if [[ "${file##*.}" == "so" ]]; then
                ln -s "../$mod_dir/$file"
            fi
        done
        popd
    fi
done
