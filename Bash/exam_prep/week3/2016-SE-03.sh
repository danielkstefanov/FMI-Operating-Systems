# Напишете shell скрипт който, ако се изпълнява от root, проверява кои потребители на
# системата нямат homedir или не могат да пишат в него.

#!/bin/bash

if [[ $(whoami) != "s0600397" ]]; then
    echo "This script can be run only as root"
    exit 1
fi

while read line; do
    user=$(echo "${line}" | cut -d':' -f1)
    homeDir=$(echo "${line}" | cut -d':' -f 6 | grep -oE  "^/home/.*$");
    if [[ $homeDir == "" ]]; then
        echo ${user}
    else
        perm=$(stat -c "%a" ${homeDir} | cut -c 1)

        if [[ ${perm} -ne 7 && ${perm} -ne 6 && ${perm} -ne 4 ]] ; then
                echo "${user}"
        fi
    fi
done < <(cat /etc/passwd)


# Примерен формат:
# root:x:0:0:root:/root:/bin/bash
# daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
# bin:x:2:2:bin:/bin:/usr/sbin/nologin