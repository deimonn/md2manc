#!/bin/sh

export INDENT_PROFILE="$MESON_SOURCE_ROOT/.indent.pro"

for arg; do
    if ! indent -st "$arg" | sed 's/{ 0 }/{0}/' | diff --color=always "$arg" -; then
        echo "make the above changes to '$arg'"
    fi
done
