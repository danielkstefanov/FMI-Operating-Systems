#!/bin/bash

# Да се напише shell скрипт, който приема точно един параметър и проверява
# дали подаденият му параметър се състои само от букви и цифри.

if ([[ $# -ne 1 ]]); then
    echo "Invalid params count";
    exit 1;
fi

if echo $1 | grep -E '[^a-zA-Z0-9]+' then;
    echo "Invalid param";
fi

echo "Valid invoke $1";