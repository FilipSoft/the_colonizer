#!/bin/bash

SRC_DIR="${1}"
DEST_DIR="${2}"

# Ensure destination directory exists
mkdir -p "$DEST_DIR"

echo "Monitoring $SRC_DIR for create, delete, and modify events..."

inotifywait -m -e create -e delete -e modify -e moved_from --format '%e %f' "$SRC_DIR" | while read -r event filename; do
    src_file="$SRC_DIR/$filename"
    dest_file="$DEST_DIR/${filename::-4}.h"

    case "$event" in
        DELETE*|MOVED_FROM*)
            if [[ -f "$dest_file" ]]; then
                echo "File deleted: $filename — removing $dest_file"
                rm -f "$dest_file"
            fi
            ;;
        CREATE*|MODIFY*)
            if [[ -f "$src_file" ]]; then
                echo "File $event: $filename — generating $dest_file"
                xxd -i "$src_file" > "$dest_file"
            fi
            ;;
    esac
done