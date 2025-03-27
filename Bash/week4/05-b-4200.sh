#!/bin/bash

# Имате компилируем (a.k.a няма синтактични грешки) source file на езика C. 
# Напишете shell script, който да покaзва колко е дълбоко най-дълбокото nest-ване (влагане).

if [[ $# -ne 1 ]]; then
    echo "Expected one argument"
    exit 1
fi

current=0
max=0

while read ch;
do 

    if [[ $ch == '{']]; then
        current=$((current + 1))
    elif [[ $ch == '}']] then
        current=$((current - 1))
    fi

    if [[ $current -gt $max ]]; then
        max=${current};
    fi
done < <(cat "${1}" | grep -E -o "[{}]")

echo "Max depth is: ${max}"