# ChordPro е текстов формат за представяне на текстове на песни, анотирани с акорди. Примерен откъс:
# For [Fmaj7]here am I [Em]sitting in a tin can
# [Fmaj7]Far above the [Em]world
# [Bb]Planet Earth is [Am]blue and there's [G]nothing I can [F]do
# Всяка поредица от символи, оградена в квадратни скоби, ще наричаме акорд. Първите един или два
# символа на акорда наричаме основен тон.
# Напишете скрипт transpose.sh, който приема един аргумент (неотрицателно число 𝑁). Скриптът
# трябва да чете текст във формàта ChordPro от stdin и да изписва аналогичен текст на stdout, заменяйки единствено основните тонове по следната схема:
# A Bb B C Db D Eb E F Gb G Ab
# Числото 𝑁 задава брой преходи по ребрата за замяна. Например, при изпълнение на скрипта с 𝑁 = 3
# върху горния пример, резултатът би бил:
# For [Abmaj7]here am I [Gm]sitting in a tin can
# [Abmaj7]Far above the [Gm]world
# [Db]Planet Earth is [Cm]blue and there's [Bb]nothing I can [Ab]do


#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Expected one arg: number"
    exit 1
fi

if ! echo "${1}" | grep "^[1-9][0-9]*$"; then
    echo "Expected positive number"
    exit 2
fi

validAccords="A Bb B C Db D Eb E F Gb G Ab"
accordsCount=12


fromToneToIndex() {
    if [[ "${1}" == "A" ]]; then
        echo "0"
    elif [[ "${1}" == "Bb" ]]; then
        echo "1"
    elif [[ "${1}" == "B" ]]; then
        echo "2"
    elif [[ "${1}" == "C" ]]; then
        echo "3"
    elif [[ "${1}" == "Db" ]]; then
        echo "4"
    elif [[ "${1}" == "D" ]]; then
        echo "5"
    elif [[ "${1}" == "Eb" ]]; then
        echo "6"
    elif [[ "${1}" == "E" ]]; then
        echo "7"
    elif [[ "${1}" == "F" ]]; then
        echo "8"
    elif [[ "${1}" == "Gb" ]]; then
        echo "9"
    elif [[ "${1}" == "G" ]]; then
        echo "10"
    elif [[ "${1}" == "Ab" ]]; then
        echo "11"
    fi
}

fromIndexToTone() {
    if [[ "${1}" == "0" ]]; then
        echo "A"
    elif [[ "${1}" == "1" ]]; then
        echo "Bb"
    elif [[ "${1}" == "2" ]]; then
        echo "B"
    elif [[ "${1}" == "3" ]]; then
        echo "C"
    elif [[ "${1}" == "4" ]]; then
        echo "Db"
    elif [[ "${1}" == "5" ]]; then
        echo "D"
    elif [[ "${1}" == "6" ]]; then
        echo "Eb"
    elif [[ "${1}" == "7" ]]; then
        echo "E"
    elif [[ "${1}" == "8" ]]; then
        echo "F"
    elif [[ "${1}" == "9" ]]; then
        echo "Gb"
    elif [[ "${1}" == "10" ]]; then
        echo "G"
    elif [[ "${1}" == "11" ]]; then
        echo "Ab"
    fi
}

while read line; do
    currentLineGroups=$(mktemp)
    echo "${line}" | grep -Eo "\[[[:alnum:]]+\]" > "${currentLineGroups}"

    while read group; do
        mainTone=$(echo "${group}" | cut -c2- | grep -Eo "(A|Bb|C|Db|D|Eb|E|F|Gb|G|Ab)")
        mainToneIndex=$(fromToneToIndex "${mainTone}")
        newToneIndex=$(echo "(${mainToneIndex} + ${1}) % ${accordsCount}" | bc)
        newTone=$(fromIndexToTone "${newToneIndex}")
        line=$(echo "${line}" | sed -Ei "s/\[${mainTone}([^]]+)\]/\[${newTone}\1\]/g")
    done < <(cat "${currentLineGroups}")
    
    echo "${line}"
done
