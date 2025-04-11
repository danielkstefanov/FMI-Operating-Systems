# Напишете shell скрипт, който приема един позиционен параметър - число. Ако скриптът
# се изпълнява като root, да извежда обобщена информация за общото количество активна памет (RSS
# - resident set size, non-swapped physical memory that a task has used) на процесите на всеки потребител.
# Ако за някой потребител обобщеното число надвишава подадения параметър, да изпраща подходящи
# сигнали за прекратяване на процеса с най-много активна памет на потребителя.
# Забележка: Приемаме, че изхода в колоната RSS е число в същата мерна единица, като числото, подадено като аргумент.

#!/bin/bash

if [[ $(whoami) != "s0600397" ]]; then
    echo "This program can be runned only by root"
    exit 1
fi

if [[ $# -ne 1 ]]; then
    echo "Expected one arg"
    exit 2
fi

if ! echo ${1} | grep -qE "^([1-9][0-9]*|0)$"; then
    echo "Expected arg to be number"
    exit 3
fi

table=$(mktemp)
ps -eo uid=,pid=,rss= >> ${table}

users=$(cat "${table}" | awk '{ print $1 }' | sort | uniq )

for user in ${users}; do
    totalRss=$(cat "${table}" | grep -E "${user}" | awk ' BEGIN { sum=0 } { sum+=$3 } END { print sum; }')
    if [[ "${totalRss}" -gt "${1}" ]] then
        highestPid="$( cat "${table}" | grep -E "${user}" | sort -t' ' -n -k3 | tail -n 1 | cut -d' ' -f2)"
        echo $highestPid
        #   kill "$(highestPid)"
    fi
done

rm ${table}

# Примерен формат:

# USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
# root 1 0.0 0.0 15816 1884 ? Ss May12 0:03 init [2]
# root 2 0.0 0.0 0 0 ? S May12 0:00 [kthreadd]
# root 3 0.0 0.0 0 0 ? S May12 0:02 [ksoftirqd/0]
# Забележка: Алтернативно може да ползвате изхода от ps -e -o uid,pid,rss