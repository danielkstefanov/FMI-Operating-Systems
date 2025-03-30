# Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива.
# Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива.

# Да се дефинира променлива BACKUP_DIR (или друго име), в която:
# - изтритите файлове се компресират и запазват
# - изтритите директории се архивират, комприсират и запазват
# - имената на файловете е "filename_yyyy-mm-dd-HH-MM-SS.{gz,tgz}", където filename е оригиналното име на файла (директорията) преди да бъде изтрит

# а) Добавете параметър -r на скрипта, който позволява да се изтриват непразни директории рекурсивно и съответно да се запазят в BACKUP_DIR


#!/bin/bash

rec=0

if [[ "${1}" == "-r" ]] ; then
        rec=1
        shift
fi

while [[ $# -ne 0 ]] ; do
    file="${1}"
    currentDate=$(date +'%Y-%m-%d-%H-%M-%S')

    if [[ -f "${file}" ]] ; then

            tar -czf "${BACKUP_DIR}/${file}_${currentDate}.gz" "${file}"
            rm ${file}
    
    elif [[ -d "${file}" ]] ; then

        dirName=$(echo "${file}" | sed -E 's/(.*)\//\1/g')
        filesCount=$(find "${file}" -mindepth 1 | wc -l)

        if [[ ${filesCount} -eq 0 ]] ; then
            tar -czf "${BACKUP_DIR}/${dirName}_${currentDate}.tgz" "${file}"
            rmdir ${file}
        else
            if [[ ${rec} -eq 1 ]] ; then
                tar -czf "${BACKUP_DIR}/${dirName}_${currentDate}.tgz" "${file}"
                rm -rf ${file}
            fi
        fi
    fi
    shift
done