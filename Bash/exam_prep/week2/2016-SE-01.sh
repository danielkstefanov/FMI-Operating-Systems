# Напишете shell скрипт, който по подаден един позиционен параметър, ако този параметър
# е директория, намира всички symlink-ове в нея и под-директориите ѝ с несъществуващ destination.

#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Expected one arg: dir"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "Expected directory"
    exit 2
fi

for file in $(find "${1}" -type l); do
    if [[ ! -e "${file}" ]]; then
        echo "${file}"
    fi
done


