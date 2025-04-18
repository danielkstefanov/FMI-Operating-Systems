# Напишете скрипт baseline.sh, който уведомява потребителя, ако текущата стойност на дадена величина сe различава много от историческите ѝ стойности, измерени предни седмици.
# baseline.sh трябва да приема 2 аргумента:
# • външна команда, измерваща търсената стойност: при изпълнение, тази команда изписва число
# с десетична точка
# • път до файл, в който baseline.sh трябва да съхранява историческите данни във формат по ваш
# избор
# Приемете, че потребителят е подсигурил, че baseline.sh ще бъде изпълняван на всеки час със еднакви аргументи. Скриптът трябва да изпълни външната команда, съхранявайки върнатата стойност и
# текущото време във файла с исторически данни. За всяко такова изпълнение, разглеждаме историческите стойности за същия ден от седмицата и час:
# • Ако командата е завършила неуспешно, скриптът не извежда нищо и приключва със статус 3
# • Ако текущата стойност се различава повече от двойно от средната историческа стойност, скриптът трябва да приключи със статус 2, след като изведе съобщение от вида:
# YYYY-MM-DD HH: VVV.VVVV abnormal
# където YYYY-MM-DD HH са датата и часа на измерването, а VVV.VVVV е измерената стойност
# • В противен случай, скриптът не извежда нищо
# Например, ако в момента е четвъртък, 2024-08-22 15:08, скриптът трябва да провери средната стойност на всички измервания, направени в четвъртъци между 15 и 16 часа. Ако тази средна стойност е
# 𝛼, а текущата стойност е 𝜉, и не е вярно, че 𝛼
# 2 ≤ 𝜉 ≤ 2𝛼, скриптът трябва да изведе съобщение.  
  
#!/bin/bash

if [[ ! -f "${2}" ]]; then
    echo "Second arg must be a file"
    exit 1
fi

comm="${1}"
file="${2}"
currentValue=$("${comm}" 2>/dev/null)

if [[ $? -ne 0 ]]; then
    exit 3
fi

dayOfWeek=$(date +"%w")
hour=$(date +"%H")


historicalValues=$(mktemp)
grep "${dayOfWeek} ${hour}" "${file}" | awk '{print $3}' > "${historicalValues}"

echo "${dayOfWeek} ${hour} ${currentValue}" >> "${file}"

if [[ -s "${historicalValues}" ]]; then
    rm "${historicalData}"
    exit 0
fi

sum=0
count=0

while read value; do
    sum=$(echo "${sum} + ${value}" | bc)
    count=$(( count + 1 ))
done < <(cat "${historicalValues}")

avg=$( echo "scale=4; ${sum} / ${count}" | bc)

if (( $(echo "${currentValue} < ${avg} / 2" ) | bc -l )) || \
   (( $(echo "${currentValue} > ${avg} * 2" ) | bc -l )); then

    echo "$(date +"%Y%m%e") ${hour}: $(printf "%.4f" ${currentCalue} ) abnormal"
    rm "${historicalValues}"
    exit 2
fi

rm "${historicalData}"
