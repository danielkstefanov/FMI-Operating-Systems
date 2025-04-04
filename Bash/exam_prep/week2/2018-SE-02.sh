# Напишете серия от команди, извеждащи на екрана само inode-а на най-скоро
# променения (по съдържание) файл, намиращ се в home директорията на
# потребител pesho (или нейните поддиректории), който има повече от едно име.

find $(cat /etc/passwd | awk -F':' -v user="pesho" ' $1 == user { print $6 }') -type f -links +1 -printf "%T@ %i \n" | sort -t' ' -k1 -nr | head -n 1 | awk '{ print $2 }'