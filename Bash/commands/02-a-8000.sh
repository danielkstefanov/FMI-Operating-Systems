#Копирайте всички файлове от /etc, които могат да се четат от всички, в
#директория myetc в home директорията ви. Направете такава, ако нямате.

mkdir ~/myetc
find /etc -type f -perm -u+r,g+r,o+r -exec cp {} ~/myetc \;