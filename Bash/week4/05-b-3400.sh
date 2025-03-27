#!/bin/bash

# Да се напише shell скрипт, който чете от стандартния вход име на файл и символен низ,
# проверява дали низа се съдържа във файла и извежда на стандартния изход кода на завършване
# на командата с която сте проверили наличието на низа.

# NB! Символният низ може да съдържа интервал (' ') в себе си.

if [[ ${#} -lt 2 ]]; then
    echo "Expected more than two args"
    exit 1
fi

if [[ ! -f "${1}" ]]; then
    echo "Expected a valid file as first argument"
    exit 2
fi

filename="${1}"
shift

text="$@"

grep -q "${text}" "${filename}"
echo "$?"