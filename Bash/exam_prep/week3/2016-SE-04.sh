# В текущата директория има само обикновени файлове (без директории).
# Да се напише bash script, който приема 2 позиционни параметъра – числа,
# който мести файловете от текущата директория към нови директории
# (a, b и c, които трябва да бъдат създадени), като определен файл се
# мести към директория ’a’, само ако той има по-малко редове от първи
# позиционен параметър, мести към директория ’b’, ако редове са между
# първи и втори позиционен параметър и в ’c’ в останалите случаи.

#!/bin/bash

if [[ $# -ne 2 ]]; then
   echo "Invalid args count: expected two numbers"
   exit 1
fi
if [[ ! "${1}" =~ ^[1-9][0-9]*$ ]]; then
   echo "Invalid first arg: expected number"
   exit 2
fi
if [[ ! "${2}" =~ ^[1-9][0-9]*$ ]]; then
   echo "Invalid second arg: expected number"
   exit 2
fi

if [[ ! -d "a" ]]; then 
    	mkdir a
fi
if [[ ! -d "b" ]]; then 
    	mkdir b
fi
if [[ ! -d "c" ]]; then 
    	mkdir c
fi

while read file; do
    linesCount=$(cat ${file} | wc -l)

    if [[ $linesCount -lt ${1} ]]; then
        mv ${file} a
    elif [[ $linesCount -gt ${2} ]]; then
        mv ${file} c
    else
        mv ${file} b
    fi
done < <(find . -type f)
