# Вашите колеги от съседната лаборатория имат нужда да автоматизират периодичното създаване на
# определен вид конфигурационни файлове на база на съществуващ текстови файл (map.txt) с изисквания. Всеки ред на файла гарантирано се състои от три думи (малки латински букви), разделени с
# whitespace. Примерно съдържание на файла:
# alfa haydn degas
# bravo rossini renoir
# charlie schubert monet
# delta berlioz monet
# echo chopin renoir
# foxtrot liszt monet
# Семантично думите на всеки ред дефинират:
# • име на сървър (hostname) – alfa, bravo, charlie...
# • име на зона (composer) – haydn, rossini, schubert...
# • име на екип (artist) – degas, renoir, monet...
# Общо файлът map.txt описва множество от екипи, като всеки екип съдържа един или повече сървъра;
# а всеки сървър има точно една зона.
# Помогнете на колегите си, като напишете bash скрипт foo.sh, който приема два позиционни параметъра – низ, дефиниращ domain и име на файл с описание.
# Скриптът трябва да извежда на стандартния изход редове, съдържащи релация за всяка зона към
# всеки сървър на съответния екип в указания по-долу формат. Имената на сървърите са допълнени с
# подадения domain и завършват с точка, като редовете с релациите за даден екип се предхождат от ред,
# започващ със знак за коментар (;) с името на екипа.

#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Invalid args count:expected two"
    exit 1
fi

if [[ ! -n "${1}" ]]; then
    echo "Invalid first arg"
    exit 1
fi

if [[ ! -f "${2}" ]]; then
    echo "Invalid second arg"
    exit 1
fi

artists=$(mktemp)

cat "${2}" | awk '{ print $3 }' | sort | uniq > ${artists}

while read artist; do
    echo "; team ${artist}"

    while read zone; do
        while read server; do

            echo "${zone} IN NS ${server}.${1}."

        done < <(cat "${2}" | awk -v searchArtist=${artist} ' $3==searchArtist { print $1}')

    done < <(cat "${2}" | awk -v searchArtist=${artist} ' $3==searchArtist { print $2}')

done < <(cat "${artists}")

# Примерно извикване и изход на база горния файл:
# $ foo.sh example.net map.txt
# ; team degas
# haydn IN NS alfa.example.net.
# ; team monet
# schubert IN NS charlie.example.net.
# schubert IN NS delta.example.net.
# 39
# schubert IN NS foxtrot.example.net.
# berlioz IN NS charlie.example.net.
# berlioz IN NS delta.example.net.
# berlioz IN NS foxtrot.example.net.
# liszt IN NS charlie.example.net.
# liszt IN NS delta.example.net.
# liszt IN NS foxtrot.example.net.
# ; team renoir
# rossini IN NS bravo.example.net.
# rossini IN NS echo.example.net.
# chopin IN NS bravo.example.net.
# chopin IN NS echo.example.net.
