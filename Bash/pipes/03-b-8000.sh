# Вземете факултетните номера на студентите (описани във файла
# <РЕПО>/exercises/data/mypasswd.txt) от СИ и ги запишете във файл si.txt сортирани.
# Студент е част от СИ, ако home директорията на този потребител (според
# <РЕПО>/exercises/data/mypasswd.txt) се намира в /home/SI директорията.

cat mypasswd | grep "/home/SI" |cut -d ':' -f1 | cut -c 2- | sort -nr > si.txt