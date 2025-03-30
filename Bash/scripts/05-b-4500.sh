# Да се напише shell скрипт, който получава при стартиране като параметър
# в командния ред идентификатор на потребител. Скриптът периодично (sleep(1))
# да проверява дали потребителят е log-нат, и ако да - да прекратява изпълнението си,
# извеждайки на стандартния изход подходящо съобщение.

# NB! Можете да тествате по същият начин като в 05-b-4300.txt

#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Invalid args count"
    exit 1
fi

username=$(id -un ${1} 2>/dev/null)

if [[ -z $username ]]; then
    echo "User with this id doesnt exist"
    exit 2
fi

while true; do
    sleep(1)

    if who|| grep "${username}" ; then
        exit 0
    fi
done