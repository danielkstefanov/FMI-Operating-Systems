# Напишете скрипт, който получава два задължителни позиционни
# параметъра – директория и низ. Сред файловете в директорията
# би могло да има такива, чиито имена имат структура

# vmlinuz-x.y.z-arch където:
# • vmlinuz е константен низ;
# • тиретата “-” и точките “.” присъстват задължително;
# • x е число, version;
# • y е число, major revision;
# • z е число, minor revision;
# • наредената тройка x.y.z формира глобалната версия на ядрото;
# • arch е низ, архитектура (платформа) за която е съответното ядро.

# Скриптът трябва да извежда само името на файла, намиращ се
# в подадената директория (но не и нейните поддиректории), който:
# • спазва гореописаната структура;
# • е от съответната архитектура спрямо параметъра-низ, подаден на скрипта;
# • има най-голяма глобална версия.

#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Invalid args count: expected two"
    exit 1
fi

if [[ ! -d "${1}" ]]; then
    echo "Invalid first arg: expected dir"
    exit 2
fi

bversion=0
bmajor=0
bminor=0
while read wholeVersion; do
    version=$(echo "${wholeVersion}" | cut -d'.' -f1)
    major=$(echo "${wholeVersion}" | cut -d'.' -f2)
    minor=$(echo "${wholeVersion}" | cut -d'.' -f3)

    better=0

    if [[ $version -gt $bversion ]]; then
        better=1
    elif [[ $version -eq $bversion && $major -gt $bmajor ]]; then
        better=1

        if [[ $bmajor -eq $major && $minor -gt $bminor ]]; then
            better=1
        fi
    fi

    if [[ $better -eq 1 ]]; then
        bversion=$version
        bmajor=$major
        bminor=$minor
    fi

done < <(find "${1}" -mindepth 1 -maxdepth 1 | cut -d'/' -f2 | awk -F'-' -v architecture="${2}" ' BEGIN { OFS="-" } $1=="vmlinuz" && $3==architecture { print $2 }')

echo "vmlinuz-${bversion}.${bmajor}.${bminor}-${2}"


# Пример:
# • Съдържание на ./kern/:
# vmlinuz-3.4.113-amd64
# vmlinuz-4.11.12-amd64
# vmlinuz-4.12.4-amd64
# vmlinuz-4.19.1-i386

# • Извикване и изход:
# $ ./task1.sh ./kern/ amd64
# vmlinuz-4.12.4-amd64