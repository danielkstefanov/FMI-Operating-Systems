# Да се напише shell скрипт, който приема файл и директория.
# Скриптът проверява в подадената директория и нейните под-директории
# дали съществува копие на подадения файл и отпечатва имената на
# намерените копия, ако съществуват такива.

# NB! Под 'копие' разбираме файл със същото съдържание.

#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Expected two args"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "Expect first arg to be file"
    exit 2
fi

if [[ ! -d $2 ]]; then
    echo "Expect second arg to be dir"
    exit 2
fi

while read file; do
    if [[ "$1" -ef "$file" ]]; then
        echo "${file}"
    fi
done < <(find "$2" -type f)  