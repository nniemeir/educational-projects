#!/bin/bash

if ! command -v gzip &>/dev/null; then
    echo "Missing dependency: gzip"
    exit 1
fi

if ! sudo cp -f leisure /usr/bin/; then
    echo "Failed to move script to /usr/bin"
    exit 1
fi

CONFIG_DIR="$HOME/.config/leisure/"

if [ ! -d "$CONFIG_DIR" ]; then
    if ! mkdir -p "$CONFIG_DIR"; then
        echo "Failed to make configuration directory."
        exit 1
    fi
    if ! cp -r config/previews/ "$CONFIG_DIR/"; then
        echo "Failed to copy previews directory."
        exit 1
    fi
fi

if ! [ -f "$CONFIG_DIR/preferences.conf" ]; then
    if ! cp config/preferences.conf "$CONFIG_DIR"; then
        echo "Failed to copy preferences.conf to configuration directory"
        exit 1
    fi
fi

if ! [ -f "$CONFIG_DIR/games.csv" ]; then
    if ! cp config/games.conf "$CONFIG_DIR"; then
        echo "Failed to copy games.csv template to configuration directory"
        exit 1
    fi
fi

if ! gzip < man/leisure.1 > man/leisure.1.gz; then
    echo "Failed to compress manpage."
fi

if ! sudo cp -f man/leisure.1.gz /usr/share/man/man1/; then
    echo "Failed to install manual page."
    exit 1
fi

if ! rm -rf man/leisure.1.gz; then
    echo "Failed to remove temporary files."
    exit 1
fi

echo "Install successful."
