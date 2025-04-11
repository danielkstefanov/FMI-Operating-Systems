# Напишете скрипт, който приема три задължителни позиционни параметра - директория
# SRC, директория DST (която не трябва да съдържа файлове) и низ АBC. Ако скриптът
# се изпълнява от root потребителя, то той трябва да намира всички файлове в
# директорията SRC и нейните поддиректории, които имат в името си като под-низ АBC,
# и да ги мести в директорията DST, запазвайки директорийната структура
# (но без да запазва мета-данни като собственик и права, т.е. не ни интересуват
# тези параметри на новите директории, които скриптът би генерирал в DST).

#!/bin/bash

if [[ $(whoami) != "s0600397" ]]; then
    echo "This program can only be runned as root"
    exit 1
fi

if [[ $# -ne 3 ]]; then
    echo "Invalid args count: expected 3 args"
    exit 2
fi


if [[ ! -d "${1}" && ! -d "${2}" ]]; then
    echo "First two params must be args!"
    exit 3
fi

if [[ $(find "${2}" -mindepth 1 | wc -l ) -ne 0 ]]; then
    echo "Destination dir must be empty"
    exit 4
fi

while read line; do
   newFileName=$(echo "${line}" | sed -E "s/^[A-Za-z0-9_]*/${2}/")
   dirs=$(dirname "${newFileName}")
   mkdir -p "${dirs}"
   mv "${line}" "${newFileName}"
done < <(find "${1}" -type f | grep -E "${3}")


# Пример:
# • в SRC (/src) има следните файлове:
# /src/foof.txt
# /src/1/bar.txt
# /src/1/foo.txt
# /src/2/1/foobar.txt
# /src/2/3/barf.txt
# • DST (/dst) е празна директория
# • зададения низ е foo

# Резултат:
# • в SRC има следните файлове:
# /src/1/bar.txt
# /src/2/3/barf.txt
# • в DST има следните файлове:
# /dst/foof.txt
# /dst/1/foo.txt
# /dst/2/1/foobar.txt