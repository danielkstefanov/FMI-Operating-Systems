# Казваме, че командата foo заема памет 𝑋, когато 𝑋 е
# сумата на заетата физическа памет (rss) на
# всички процеси, чиято команда (comm) е foo.
# Напишете скрипт, който поглежда процесите на системата
# през една секунда, докато спрат да съществуват команди,
# чиято заета памет е над 65536. След това, скриптът трябва
# да изведе всички команди, които са заемали памет
# над 65536 в поне половината “поглеждания”

#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Expected one arg"
    exit 1
fi

if ! echo "${1}" | grep -E "^([1-9][0-9]*|0)$" ;then
   echo "Invalid first arg: expected num"
   exit 1
fi

bigCommands=$(mktemp)
count=0

psRes=$(mktemp)
ps -eo comm=,rss= > "${psRes}"


while true; do
    count=$(( count + 1 ))
    break=true

    while read command; do
        totalRss=$(cat "${psRes}" | grep -E "^${command}" | awk '{ print $2 }' | xargs | tr ' ' '+' | bc)

        if [[ "${totalRss}" -gt ${1} ]]; then
            echo "${command}" >> "${bigCommands}"
            break=false
        fi

    done < <(cat "${psRes}" | awk '{ print $1 }' | sort | uniq)

    if [[ "${break}" == "true" ]]; then
        break
    fi

    sleep 1
    ps -eo comm=,rss= > "${psRes}"
done

echo "${bigCommands}" | sort | uniq -c | awk -v count="${count}" ' $1 > count/2 '

rm "${bigCommands}"
rm "${psRes}"

