# Този път програмата ви ще приема само един параметър, който е измежду ("-r", "-b", "-g", "-x").
# Напишете shell script, който приема редовете от stdin и ги изпринтва всеки ред с редуващ се цвят.
# Цветовете вървят RED-GREEN-BLUE и цветът на първия ред се определя от аргумента.
# Ако е подаден аргумент "-x", то не трябва да променяте цветовете в терминала (т.е., все едно сте извикали командата cat).



#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Expected one arg"
    exit 1
fi

redcode='\033[0;31m'
greencode='\033[0;32m'
bluecode='\033[0;34m'
defaultcode='\033[0m'

if [[ ${1} == '-r' ]]; then
    code=${redcode}
elif [[ ${1} == '-g' ]]; then
    code=${greencode}
elif [[ ${1} == '-b' ]]; then
    code=${bluecode}
elif [[ ${1} == '-x' ]]; then
    code=${defaultcode}
fi

while read row; do
    echo -e "${code} ${row}"  

    if [[ "${code}" == "${redcode}" ]]; then
        code=${greencode}
    elif [[ ${code} == ${greencode} ]]; then
        code=${bluecode}
    elif [[ ${code} == ${bluecode} ]]; then
        code=${redcode}
    fi 
done

echo -e "${defaultcode}"

# Hint: Не забравяйте да връщате цветовете в терминала.