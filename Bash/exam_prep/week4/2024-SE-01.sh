# Министерството на Истината ви поставя спешна задача:
# да напишете скрипт replace.sh, който замества думи в определени текстове.
# replace.sh приема следните два типа валидни аргументи:
# • Файл: произволен низ, не-започващ с тире, интерпретиран като име на файл
# • Замяна: аргумент от вида -R<дума1>=<дума2>
# Редът на аргументите няма значение, а броят им може да е произволен.
# За всяка замяна, replace.sh трябва да промени всички файлове,
# така че всички срещания на <дума1> да бъдат заменени с <дума2>.

# Забележки за поведението на скрипта:
# • Приемаме, че думите съдържат само латински букви и цифри,
#   а замяната счита главните и малките букви за различни (case sensitive).
# • Заменят се само пълни думи
# • Вече заменена дума не се заменя втори път. Решения,
#   не-спазващи това условие, се оценяват с най-много 8 точки.
# • Не може да разчитате, че фиксиран от вас низ не се среща в никой текст.

# Ако ви трябва низ, който гарантирано не се среща никъде, може да използвате pwgen(1)

#!/bin/bash

dictionary=$(mktemp)
files=$(mktemp)

for arg in "$@"; do
    if echo "${arg}" | grep -qE "^-R[[:alnum:]]+=[[:alnum:]]+"; then
        echo "${arg}" | cut -c3- >> "${dictionary}"
    elif echo "${arg}" | grep -Eqv "^-.*" && [[ -f "${arg}" ]]; then
        echo "${arg}" >> "${files}"
    else
        echo "Invalid arg"
        exit 1
    fi
done

random=$(pwgen)

while read file; do
    while read entry; do
        replace=$(echo "${entry}" | cut -d'=' -f1)
        replacement=$(echo "${entry}" | cut -d'=' -f2)
        sed -iE "/^#/! s/\b${replace}\b/${replacement}${random}/g" "${file}"
    done < <(cat "${dictionary}")
    sed -iE "s/${random}//g" "${file}"
done < <(cat "${files}")

rm "${dictionary}"
rm "${files}"


# Пример:
# $ cat propaganda.txt
# Our ally Eastasia is helping us in the war effort against Eurasia.
# $ ./replace.sh -REastasia=Eurasia -REurasia=Eastasia propaganda.txt
# $ cat propaganda.txt
# Our ally Eurasia is helping us in the war effort against Eastasia.

# Бонус: Скриптът да не извършва никаква замяна в редове, които започват със символа #
