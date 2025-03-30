# Да се напише shell скрипт, който намира броя на изпълнимите файлове в PATH.
# Hint: Предполага се, че няма спейсове в имената на директориите
# Hint2: Ако все пак искаме да се справим с този случай, да се разгледа IFS променливата и констуркцията while read -d

#!/bin/bash

count=0

while read dir; do
    filesCountInThisDir=$(find ${dir} -executable | wc -l)
    count=$(( filesCountInThisDir + count ))
done < <(echo $PATH | grep -Eo '[^:]+')

echo $count
