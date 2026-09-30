#!/usr/bin/env python

# compile_flags.py: Simple script for Meson to generate compile_flags.txt files
# (https://github.com/deimonn/compile_flags.py)

# Copyright (c) 2026 Deimonn
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from os import getenv
from sys import argv
from typing import Callable

import json

def error(message: str):
    print('compile_flags.py: error: ' + message)

def single[T](haystack: list[T], predicate: Callable[[T], bool]) -> T | None:
    for needle in haystack:
        if predicate(needle):
            return needle

    return None

build_root = getenv("MESON_BUILD_ROOT")
source_root = getenv("MESON_SOURCE_ROOT")

target_name = None
output_path = "compile_flags.txt"

if build_root == None or source_root == None:
    error('this script is meant to be run by Meson (MESON_BUILD_ROOT and/or '
          'MESON_SOURCE_ROOT missing from environment)')
    exit(2)

if len(argv) >= 2:
    target_name = argv[1]

if len(argv) >= 3:
    output_path = argv[2]

if len(argv) > 3:
    error('too many arguments: at most accepts a target name and output path')
    exit(2)

input = build_root + '/meson-info/intro-targets.json'
output = source_root + '/' + output_path

with open(input, 'r') as file:
    intro_targets = json.load(file)

if target_name:
    intro_target = single(intro_targets, lambda x: x['name'] == target_name)

    if intro_target == None:
        error('no such target: ' + target_name)
        exit(1)
else:
    intro_target = intro_targets[0]

target_sources = single(intro_target['target_sources'],
                        lambda x: x['language'] in ['c', 'cpp'])

if target_sources == None:
    error('specified or first target has no C/C++ compilation flags')
    exit(1)

language = target_sources['language']
parameters = target_sources['parameters']

if language == 'cpp':
    parameters = ['-xc++'] + parameters
else:
    parameters = ['-xc'] + parameters

with open(output, "w") as file:
    for parameter in parameters:
        file.write(parameter)
        file.write("\n")
