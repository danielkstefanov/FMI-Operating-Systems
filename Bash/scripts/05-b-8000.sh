# Напишете shell script, който получава като единствен аргумент
# име на потребител и за всеки негов процес изписва съобщение за
# съотношението на RSS към VSZ. Съобщенията да са сортирани, като
# процесите с най-много заета виртуална памет са най-отгоре.

# Hint:
# Понеже в Bash няма аритметика с плаваща запетая, за смятането на
# съотношението използвайте командата bc. За да сметнем
# нампример 24/7, можем да: echo "scale=2; 24/7" | bc

# Резултатът е 3.42 и има 2 знака след десетичната точка, защото scale=2.
# Алтернативно, при липса на bc ползвайте awk.

#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Expected one argument"
    exit 1
fi

grep -qE "^${1}:" /etc/passwd

if [[ $? -ne 0 ]]; then
    echo "Invalid username"
    exit 2
fi

while read processInfo; do
    pid=$(echo $processInfo | cut -d ' ' -f1)
    rss=$(echo $processInfo | cut -d ' ' -f2)
    vsz=$(echo $processInfo | cut -d ' ' -f3)
    ratio=$(echo "scale=2; ${rss}/${vsz}" | bc)
    echo " Process: ${pid} with rss/vsz= ${ratio}"
done < <(ps -o pid=,rss=,vsz= -U ${1} | tr -s [:space:] | sed 's/^ //g' | sort -nr -t ' ' -k3)
