# Да се напише shell скрипт, който приема произволен брой
# аргументи - имена на файлове или директории. Скриптът
# да извежда за всеки аргумент подходящо съобщение:

# 	- дали е файл, който може да прочетем
# 	- ако е директория - имената на файловете в нея, които имат размер, по-малък от броя на файловете в директорията.

#!/bin/bash

for obj in ${@}; do
    if [[ ! -f $obj && ! -d $obj ]]; then
        echo "Expected only files and dirs as args"
        exit 1
    fi
done

for obj in ${@}; do

    if [[ -f $obj ]]; then
        if [[ -r $obj ]]: then
            echo "${obj} is readable"
        else
            echo "${obj} in not readable"
        fi
    else
        filesCount=$(ls "${obj}" | wc -l)

        for file in $(find ${obj} -type f -size -${filesCount}c); do
            echo "${file}"
        done
    fi
done
