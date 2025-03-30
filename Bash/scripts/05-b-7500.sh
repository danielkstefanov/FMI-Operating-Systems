# Напишете shell script guess, която си намисля число, което вие трябва да познате.
# В зависимост от вашия отговор, програмата трябва да ви казва "надолу" или "нагоре",
# докато не познате числото. Когато го познаете, програмата да ви казва с колко опита сте успели.

# ./guess (програмата си намисля 5)

# Guess? 22
# ...smaller!
# Guess? 1
# ...bigger!
# Guess? 4
# ...bigger!
# Guess? 6
# ...smaller!
# Guess? 5
# RIGHT! Guessed 5 in 5 tries!


#!/bin/bash

min=0
max=10

if [[ $# -ne 2 && $# -ne 0 ]]; then
    echo "Invalid args count! Expected zero or two args"
    exit 1
fi

if [[ $# -eq 2 ]]; then
    min=$1
    max=$2
fi

if [[ $min -gt $max ]]; then
    echo "Invalid interval"
    exit 2
fi

answer=$(( (RANDOM % (max - min + 1)) + min ))

while read -p "Guess: " num; do
    if [[ ! $num =~ ^[0-9]+$ ]]; then
        echo "Expect only nums as guesses"
    else
        if [[ $answer -eq $num ]]; then
            echo "YOU WIN!!!!!!!"
            exit 0
        else
            if [[ $answer -gt $num ]]; then
                echo "...bigger"
            else
                echo "...smaller"
            fi
        fi
    fi
done

# Hint: Един начин да направите рандъм число е с $(( (RANDOM % b) + a  )),
# което ще генерира число в интервала [a, b]. Може да вземете a и b като
# параметри, но не забравяйте да направите проверката.

