# Напишете скрипт, който копира снимки от дадена директория (която ще наричаме фотоапарата) в
# нова директория (която ще наричаме библиотеката), спазвайки определена структура.
# Скриптът приема пътищата на двете директории като аргументи, очаквайки, че библиотеката не
# съществува и трябва да я създаде.
# Снимките са всички файлове в директорията на фотоапарата и нейните поддиректории, чиито
# имена завършват на .jpg. Под дата на снимка ще имаме предвид нейното време на модификация
# (mtime).
# Библиотеката трябва да се състои от поддиректории с имена от вида 2023-04-20_2023-04-23, означаващи затворени интервали от дни. Скриптът трябва да създаде такива поддиректории и да разположи снимките в тях, така че да са изпълнени следните свойства:
# 1. Всяка снимка се намира в такава директория, че датата на снимката да принадлежи на съответния интервал от дни
# 2. За всеки ден от всеки интервал съществува поне една снимка от този ден в този интервал
# 3. Няма припокриващи се и долепени интервали (между всеки два различни интервала има поне
# един ден без снимка)
# Оригиналните имена на снимките не ни интересуват: скриптът трябва да им даде нови имена, използвайки техния mtime, с формат, изглеждащ така:
# <библиотеката>/2023-04-20_2023-04-23/2023-04-21_13:04:33.jpg
# Подсказка:
# $ date -d '2000-02-29 + 1 day' +'%Y-%m-%d'
# 2000-03-01
# Бонус за още няколко точки: направете скрипта така, че да може да работи при вече съществуваща
# библиотека, добавяйки снимките от фотоапарата към нея и запазвайки всички свойства. Ако снимка
# с дадено име вече съществува в библиотеката, нека скриптът я пропусне.

#!/bin/bash

if [[ ! -d "${1}" ]]; then
    echo "Invalid first arg: expected dir"
    exit 1
fi

if [[ ! -d "${2}" ]]; then
    mkdir "${2}"
fi

libDir="${2}"
photosForCurrentDir=$(mktemp)
prevPhotoDate=""

createDir() {
    startDate=$(cat "${photosForCurrentDir}" | head -n 1 | cut -d ' ' -f 2 | cut -d '_' -f 1)
    endDate=$(cat "${photosForCurrentDir}" | tail -n 1 | cut -d ' ' -f 2 | cut -d '_' -f 1)
    dirName="${libDir}/${startDate}_${endDate}"

    mkdir "${dirName}"

    while read photo; do
        mv "$(echo "${photo}" | awk '{ print $1 }')" "${dirName}/$(echo "${photo}" | awk '{ print $2 }')"
   done < <(cat "${photosForCurrentDir}")

    rm "${photosForCurrentDir}"
    photosForCurrentDir=$(mktemp)
}


while read line; do

    photoName=$(echo "${line}" | cut -d" " -f1)
    photoDate=$(echo "${line}" | cut -d" " -f2 | cut -d"+" -f1)
    photoHour=$(echo "${line}" | cut -d" " -f2 | cut -d"+" -f2 | cut -d'.' -f1)
    if [[ -z "${prevPhotoDate}" ]]; then
        prevPhotoDate="${photoDate}"
    fi

    maxPossibleDate=$(date -d "${prevPhotoDate} + 1 day" +'%Y-%m-%d')

    if ! [[ "${photoDate}" == "${maxPossibleDate}" ||  "${photoDate}" == "${prevPhotoDate}" ]]; then
        createDir
    fi

    echo "${photoName} ${photoDate}_${photoHour}.jpg" >> "${photosForCurrentDir}"

    prevPhotoDate="${photoDate}"

done < <(find "${1}" -type f -name "*.jpg" -printf "%p %T+ \n" | sort -t" " -k2 -n)

if [[ -s "${photosForCurrentDir}" ]]; then
    createDir
fi

rm "${photosForCurrentDir}"
