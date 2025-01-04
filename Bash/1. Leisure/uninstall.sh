#!/bin/bash

if ! sudo rm -rf /usr/share/man/man1/leisure.1.gz; then
    echo ""
    exit 1
fi

if ! sudo rm -rf /usr/bin/leisure; then
    echo "Failed to remove script"
    exit 1
fi

echo "Uninstall successful."
