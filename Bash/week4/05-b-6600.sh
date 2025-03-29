# Да се напише shell скрипт, който получава единствен аргумент
# директория и изтрива всички повтарящи се (по съдържание) файлове
# в дадената директория. Когато има няколко еднакви файла, да се остави
# само този, чието име е лексикографски преди имената на останалите дублирани файлове.

#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Expected one argument"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "Expected a directory"
    exit 2
fi

uniquefiles=()

while read file; do

    isUnique=1

    for currentUniqueFile in "${uniquefiles[@]}"; do
        cmp -s "${file}" "${currentUniqueFile}"

        if [[ $? -eq 0 ]]; then
            isUnique=0
            break
        fi
    done

    if [[ ${isUnique} -eq 0 ]]; then 
        rm "${file}"
    else 
        uniquefiles+=("${file}")
    fi

done < <(find -type f -mindepth 1 -maxdepth 1 | sort -z)


# Примери:
# $ ls .
# f1 f2 f3 asdf asdf2
# # asdf и asdf2 са еднакви по съдържание, но f1, f2, f3 са уникални

# $ ./rmdup .
# $ ls .
# f1 f2 f3 asdf
# # asdf2 е изтрит
