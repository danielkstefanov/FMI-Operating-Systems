# Напишете скрипт, който извежда името на потребителския акаунт,
# в чиято home директория има най-скоро променен обикновен файл
# и кой е този файл. Напишете скрипта с подходящите
# проверки, така че да бъде валиден инструмент.

#!/bin/bash

if [[ $# -ne 0 ]]; then
    echo "Doesn't expect any arguments!"
    exit 1
fi

acc=""
file=""
bestTime=0

while read line; do
    homeDir=$( echo ${line} | awk  -F':' ' { print $6 } ' )

    bestFile=$(find "${homeDir}" -type f  -printf "%T@ %p \n" 2>/dev/null | sort -t' ' -nr | head -n 1)
    time=$(echo ${bestFile} | awk -F' ' ' { print $1 } ' )
    filename=$(echo ${bestFile} | awk -F' ' ' { print $2 } ' )

    echo "$time > $bestTime" | bc
    
    if [[ $? -eq 0 ]]; then
        bestTime=$time
        file=$filename
        acc=$(echo ${line} | awk -F':' ' { print $1 } ')
    fi
done < <(cat /etc/passwd)
