# Напишете shell script, който да приема параметър име на директория, от която взимаме файлове,
# и опционално експлицитно име на директория, в която ще копираме файлове. Скриптът да копира
# файловете със съдържание, променено преди по-малко от 45 мин, от първата директория във втората
# директория. Ако втората директория не е подадена по име, нека да получи такова от днешната дата във формат,
# който ви е удобен. При желание новосъздадената директория да се архивира.

#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "Expected at least one argument"
    exit 1
fi

if [[ ! -d ${1}  ]]; then
    echo "Expected first argument to be directory"
    exit 2
fi

OUTPUT=$2

if [[ ! -d $2 ]]; then
    OUTPUT=$(date -I)
fi

cp -r $(find ${FILE} -mindepth 1 -maxdepth 1 -mmin -45) ${OUTPUT}

tar -cf arhive.tar ${OUTPUT}