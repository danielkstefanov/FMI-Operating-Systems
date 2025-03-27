#!/bin/bash

# Да се напише shell скрипт, който чете от стандартния вход имената на 3 файла,
# обединява редовете на първите два (man paste), подрежда ги по азбучен ред и резултата записва в третия файл.

if [[ ${#} -nq 3]]; then
    echo "Expected three agruments"
    exit 1
fi

if [[ ! -f "${1}" || ! -f "${2}" || ! -f "${3}" ]]; then
    echo "Expected only valid filenames as arguments"
    exit 2
fi

cat "${1}" "${2}" | sort 1>${3}