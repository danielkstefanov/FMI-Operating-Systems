#!/bin/bash

if [[ ! -d "${1}" ]]; then
    echo "Invalid first arg: expected dir"
    exit 1
fi

if ! echo "${2}" | grep -E "^[1-9][0-9]*$"; then
    echo "Expected second arg to be number"
    exit 1
fi

for i in $(seq 0 3); do

    to_break="false"

    while read line; do
        backup_path=$(echo "${line}" | cut -d ' ' -f 1)
        file_name=$(basename "${backup_path}")
        server_name=$(echo "${file_name}" | cut -d ' ' -f 1 | cut -d '-' -f 1-2)
        backups_count=$(find "${1}${i}" | grep -E "${server_name}" | wc -l)

        if [[ ${backups_count} -gt $(( i + 1 )) ]]; then
            rm "${backup_path}"

            current_percentage=$(df "${1}" | tail -n 1 | awk '{print $5}' | cut -d '%' -f1)

            if [[ ${current_percentage} -lt ${2} ]]; then
                to_break="true"
                break
            fi
        fi

    done < <(find "${1}${i}" -type f -printf "%p %s %A@\n" | sort -k2nr -k3n)


    if [[ "${to_break}" == "true" ]]; then
        break
    fi

done

while read line; do
    if [[ ! -e "${line}" ]]; then
        rm "${line}"
    fi
done < <(find "${1}" -type l)
