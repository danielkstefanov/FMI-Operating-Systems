# Да се напише shell скрипт, който приема два параметъра
# - име на директория и число. Скриптът да извежда на стандартния
# изход имената на всички обикновени файлове във директорията, които
# имат размер, по-голям от подаденото число.

#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Expected two arguments"
    exit 1
fi

if [[ ! -d ${1} ]]; then
    echo "Expected dir as first arg"
    exit 2
fi

if [[ ! ${2} =~ ^[0-9]+$ ]]; then
    echo "Expected second arg to be number"
    exit 3
fi

for fileInfo in "$(find -mindepth 1 -maxdepth 1 -type f -size +${2}c -printf '%f %s \n')"; do
    echo "${fileInfo}"
done
