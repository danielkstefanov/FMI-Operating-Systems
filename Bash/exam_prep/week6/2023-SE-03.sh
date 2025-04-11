# При статистическа обработка на текстове често се налага да имаме списък от думи
# (наречени “стопдуми”), които се срещат прекалено често и не носят стойност за
# изследването. Такива думи са например “you”, “then”, “for” и т.н.
# Напишете скрипт stopword_candidates.sh, който приема като аргумент име на директория
# и извежда 10-те думи, които най-много изглеждат като стоп-думи.

# • За да бъде стоп-дума, трябва броят файлове, които я съдържат ≥ 3 пъти да е ≥ общия брой файлове / 2
# • Една дума е по-добър кандидат от друга, ако има по-голям общ брой срещания във всички файлове


# Забележки:
# • Под “всички файлове” имаме предвид всички обикновени файлове в дадената директория и
#   нейните поддиректории
# • Под “дума” имаме предвид непрекъсната последователност от латински букви (a-z) - всички
#   останали символи не са част от думите
# • За улеснение може да приемете, че във файловете няма главни букви


#!/bin/bash

if [[ ! -d "${1}" ]]; then
    echo "Invalid first arg: expected dir"
    exit 1
fi

wordInFileDict=$(mktemp)
filesCount=0

while read file; do
    filesCount=$(( filesCount + 1 ))
    cat "${file}" | grep -Eo "\b[[:alnum:]]+\b" | xargs -I word printf "%s %s\n" "word" "${file}" >> "${wordInFileDict}"
done < <(find "${1}" -mindepth 1 -type f)

filesCountDevidedByTwo=$( echo "scale=2; ${filesCount} / 2 " | bc)
wordWithCountInFileDict=$(mktemp)
cat "${wordInFileDict}" | sort | uniq -c | tr -s ' ' > "${wordWithCountInFileDict}"

uniqueWords=$(mktemp)
cat "${wordWithCountInFileDict}" | awk -F' ' '{ print $2 }' | sort | uniq > "${uniqueWords}"

stopWords=$(mktemp)

while read word; do
    countOfFilesWithMoreThanThree=$(cat "${wordWithCountInFileDict}" | awk -v word="${word}" ' $2 == word && $1 >= 3 { print $0 }' | wc -l)

    comparisonRes=$(echo "${countOfFilesWithMoreThanThree} >= ${filesCountDevidedByTwo}" | bc -l)
    if [[ ${comparisonRes} -eq 1 ]]; then
        echo "${word}" >> "${stopWords}"
    fi
done < <(cat "${uniqueWords}")

stopWordsWithOccurences=$(mktemp)

while read stopWord; do
    occurences=$(cat "${wordWithCountInFileDict}" | awk -v stopWord="${stopWord}" 'BEGIN { count=0} $2 == stopWord { count+=$1 } END { print count }')
    echo "${occurences} ${stopWord}" >> "${stopWordsWithOccurences}"
done < <(cat "${stopWords}")

cat "${stopWordsWithOccurences}" | sort -t' ' -k1 -nr | head


rm "${wordInFileDict}"
rm "${stopWordsWithOccurences}"
rm "${wordWithCountInFileDict}"
rm "${uniqueWords}"
rm "${stopWords}"
