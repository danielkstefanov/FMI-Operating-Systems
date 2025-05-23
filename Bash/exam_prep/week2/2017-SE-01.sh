#Напишете скрипт, който получава задължителен първи позиционен параметър – директория и незадължителен втори – число.
#Скриптът трябва да проверява подадената директория
#и нейните под-директории и да извежда имената на:

#а) при подаден на скрипта втори параметър – всички файлове с брой hardlink-ове поне равен на
#параметъра;
#б) при липса на втори параметър – всички symlink-ове с несъществуващ destination (счупени
#symlink-ове).
#Забележка:За удобство приемаме, че ако има подаден втори параметър, то той е число.

#!/bin/bash

if [[ $# -lt 1 ]] || [[ $# -gt 2 ]] ; then
    echo "Expected one or two arguments"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "Expected first arg to be dir"
    exit 2
fi

if [[ ! ${2} =~ ^[1-9][0-9]*$ ]]; then
    echo "Expected second arg to be number"
    exit 2
fi


if [[ $# -eq 2 ]] ; then
    find ${1} -mindepth 1 -type f 2>/dev/null   | xargs -I{} stat -c "%h %n" {} | awk -v var=${2} ' var<=$1 { print $2 } '
else
    find ${1} -mindepth 1 -type l 2>/dev/null -exec test ! -e {} \; -print
fi