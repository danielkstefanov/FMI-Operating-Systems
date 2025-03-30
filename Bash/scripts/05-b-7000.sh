# Да се напише shell скрипт, който приема произволен брой аргументи
# - имена на файлове. Скриптът да прочита от стандартния вход
# символен низ и за всеки от зададените файлове извежда по подходящ
# начин на стандартния изход броя на редовете, които съдържат низа.

#!/bin/bash

for arg in "${@}"; do
echo ${arg}
    if [[ ! -f $arg ]]; then
        echo "Expect only files as args"
        exit 1
    fi
done

read -p "Enter text to search in files: " text

for file in $@; do
    rowsCount=$(grep -wo "${text}" ${file} | wc -l)
    echo "${file}: contains text in ${rowsCount} rows"
done

# NB! Низът може да съдържа интервал.