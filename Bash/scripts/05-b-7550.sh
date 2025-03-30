# Да се напише shell скрипт, който приема параметър - име на потребител.
# Скриптът да прекратява изпълненито на всички текущо работещи процеси
# на дадения потребител, и да извежда колко са били те.

# NB! Може да тествате по същият начин като описаният в 05-b-4300

#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Expected one arg (username)"
    exit 1
fi

isValidUser=0

for user in $(cat /etc/passwd | cut -d ':' -f1); do
    if [[ "${user}" == "${1}" ]]; then
        isValidUser=1
        break
    fi
done

if [[ ${isValidUser} -eq 0 ]]; then
    echo "Invalid username"
    exit 2
fi

processes=$(ps -U "${1}" | tail -n +2 | awk ' { print $1 } ')

for process in ${processes}; do
  kill ${process}
done
