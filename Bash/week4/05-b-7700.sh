# Да се напише shell скрипт, който приема два параметъра
# - име на директория и число. Скриптът да извежда сумата
# от размерите на файловете в директорията, които имат
#    размер, по-голям от подаденото число.

#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Invalid args count"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "First arg must be dir"
    exit 2
fi

if echo ${2} | grep -Eqv "^[0-9]+$"; then
    echo "Second arg must be a number"
    exit 3
fi

sum=0

for fileSize in $(find ${1} -mindepth 1 -maxdepth 1 -type f -size +${2} -printf '%s \n'); do
    sum=$(( sum + fileSize))
done

echo ${sum}
