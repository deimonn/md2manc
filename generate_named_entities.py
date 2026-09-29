#!/usr/bin/env python

# Generates the named entity parser for md2manc

# Keep the `src/entities.json` file up-to-date with:
# https://html.spec.whatwg.org/entities.json

from sys import argv

import json

with open(argv[1]) as file:
    entity_map = json.load(file)

print('#include <stdio.h>')
print('#include <strings.h>')
print('')
print('/* LCOV_EXCL_START */')
print('')
print('int parse_named_entity(const char *entity) {')

for entity, definition in entity_map.items():
    if not entity.endswith(';'):
        continue

    print('    if (strcasecmp(entity, "' + entity + '") == 0) {')

    for codepoint in definition['codepoints']:
        print('        printf("\\\\[u%04X]", ' + str(codepoint) + ');')

    print('        return 0;')
    print('    }')
    print('')

print('    return -1;')
print('}')
print('')
print('/* LCOV_EXCL_STOP */')
