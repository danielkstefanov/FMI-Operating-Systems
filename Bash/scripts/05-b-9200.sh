# Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива.
# Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива.
# За всеки изтрит файл (директория) скриптът добавя ред във log файл с подходящо съобщение.

# а) Името на log файла да се чете от shell environment променлива, която сте конфигурирали във вашия .bashrc.
# б) Добавете параметър -r на скрипта, който позволява да се изтриват непразни директории рекурсивно.
# в) Добавете timestamp на log съобщенията във формата: 2018-05-01 22:51:36

#!/bin/bash

logfile="rmLogFile.log"

if [[ -z "${RMLOG_FILE}" ]]; then
    touch $logFile
else
    logfile="${RMLOG_FILE}"
fi

if [[ ${1} == '-r' ]]; then
    recursively="-r"
    shift
fi

for file in ${@}; do
    if [[ ! -f "${file}" && ! -d "${file}" ]]; then
        echo "Expected only files as args"
        exit 1
    elif [[ -f "${file}" ]]
        rm $file
        echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed file ${file}" >> $logfile
    else
        filesInDir=$(find ${file} | wc -l)
        if [[ $filesInDir -eq 0 ]]; then
            rmdir $file
            echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory ${file}" >> $logfile
        else
             rm ${recursively} ${file}
             echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory recursively ${file}" >> $logfile
        fi
    fi
done


# Примери:
# $ export RMLOG_FILE=~/logs/remove.log
# $ ./rmlog -r f1 f2 f3 mydir/ emptydir/
# $ cat $RMLOG_FILE
# [2018-04-01 13:12:00] Removed file f1
# [2018-04-01 13:12:00] Removed file f2
# [2018-04-01 13:12:00] Removed file f3
# [2018-04-01 13:12:00] Removed directory recursively mydir/
# [2018-04-01 13:12:00] Removed directory emptydir/