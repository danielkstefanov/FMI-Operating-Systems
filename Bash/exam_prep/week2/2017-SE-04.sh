# Напишете shell script, който получава задължителен първи позиционен
# параметър – директория и незадължителен втори – име на файл. Скриптът
# трябва да намира в подадената директория и нейните под-директории
# всички symlink-ове и да извежда (при подаден аргумент файл –
# добавяйки към файла, а ако не е – на стандартния изход) за тях следната информация:

# • ако destination-a съществува – името на symlink-а -> името на destination-а;
# • броя на symlink-овете, чийто destination не съществува.

#!/bin/bash

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "Expected one or two args"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "First args must be dir!"
    exit 2
fi

if [[ $# -eq 2 && ! -f "${2}" ]]; then
    echo "Second args must be file!"
    exit 2
fi

DIRECTORY=$1
BROKEN_LINKS=0

while read FILE TYPE ; do
    if [[ $TYPE != 'N' ]]; then
        if [[ ${#} -eq 2 ]]; then
            echo "${FILE} -> $(readlink ${FILE})" >> ${2}
        else
            echo "${FILE} -> $(readlink ${FILE})"
        fi
    else
        BROKEN_LINKS=$((BROKEN_LINKS + 1))
    fi
done < <(find ${DIRECTORY} -type l -printf "%f %Y\n" 2>/dev/null)

echo "Broken links: ${BROKEN_LINKS}"


# Примерен изход:
# lbaz -> /foo/bar/baz
# lqux -> ../../../qux
# lquux -> /foo/quux        
# Broken symlinks: 34