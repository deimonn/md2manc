#!/bin/sh

export INDENT_PROFILE="$MESON_SOURCE_ROOT/.indent.pro"

exit_code=0

for arg; do
    if ! indent -st "$arg" | sed 's/{ 0 }/{0}/' | diff --color=always "$arg" -; then
        echo "make the above changes to '$arg'"
        exit_code=1
    fi
done

exit $exit_code
