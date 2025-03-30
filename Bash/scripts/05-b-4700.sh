# Да се напише shell скрипт, който форматира големи числа, за да са по-лесни за четене.
# Като пръв аргумент на скрипта се подава цяло число.
# Като втори незадължителен аргумент се подава разделител. По подразбиране цифрите се разделят с празен интервал.

# Примери:
# $ ./nicenumber.sh 1889734853
# 1 889 734 853

# $ ./nicenumber.sh 7632223 ,
# 7,632,223

#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "Invalid args count"
    exit 1
fi

if [[ ! "${1}" =~ ^[0-9]+$ ]]; then
    echo "Expected first arg to be number"
    exit 2
fi

delimiter=" "

if [[ -n "$2" ]]; then 
    delimiter="${2}"
fi

numLength=${#1}
currentLength=${#1}
while read ch; do 
    if (( (currentLength % 3 == 0) && (currentLength != numLength) )); then
        printf "%s" "$delimiter"
    fi

    printf "%s" "$ch"
    currentLength=$(( currentLength - 1 ))
done < <(echo ${1} | grep -E -o '[0-9]')

printf "\n"