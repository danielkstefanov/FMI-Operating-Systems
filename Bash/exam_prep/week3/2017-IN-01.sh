# Напишете скрипт, който приема три задължителни позицонни аргумента:
#   • име на фаил
#   • низ1
#   • низ2

# Файлът е текстови, и съдържа редове във формат:
# ключ=стойност
# където стойност може да бъде:
# • празен низ, т.е. редът е ключ=
# • низ, състоящ се от един или повече термове, разделени с интервали, т.е., редът е ключ=𝑡1 𝑡2 𝑡3

# Някъде във файла:
# • се съдържа един ред с ключ първия подаден низ (низ1);
# • и може да се съдържа един ред с ключ втория подаден низ (низ2).

# Скриптът трябва да променя реда с ключ низ2 така, че обединението на
# термовете на редовете с ключове низ1 и низ2 да включва всеки терм еднократно.

#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Invalid args count"
    exit 1
fi

if [[ ! -f "${1}" ]]; then
    echo "invalid first arg: expected file"
    exit 2
fi

result=""

firstKeyArgs=$(cat "${1}" | egrep  "^${2}=" | awk -F '=' '{print $2}' | tr ' ' '\n')
secondKeyArgs=$(cat "${1}" | egrep  "^${3}=" | awk -F '=' '{print $2}' | tr ' ' '\n')

while read line ; do
    echo "${firstKeyArgs}" | grep -q -F "${line}"

    if [[ ${?} -ne 0 ]] ; then
        result+=$(echo "${line} ")
    fi
done < <(echo "${secondKeyArgs}")


echo ${result}
sed -i -E "s/(${3}=).*/\1${result}/g" ${1}

# Примерен входен файл:
# $ cat z1.txt
# FOO=73
# BAR=42
# BAZ=
# ENABLED_OPTIONS=a b c d
# ENABLED_OPTIONS_EXTRA=c e f
# Примерно извикване:
# $ ./a.sh z1.txt ENABLED_OPTIONS ENABLED_OPTIONS_EXTRA
# Изходен файл:
# $ cat z1.txt
# FOO=73
# BAR=42
# BAZ=
# ENABLED_OPTIONS=a b c d
# ENABLED_OPTIONS_EXTRA=e f