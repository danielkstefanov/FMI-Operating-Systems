# Разполагате с директория /var/log/my_logs, в която се намират log файлове. Log файл представлява
# обикновен файл, чието име има следния вид:

# <server_name>_<unix_timestamp>.log

# Името на сървър (server_name) може да съдържа букви, цифри и долни черти. Unix timestamp е число,
# означаващо брой изминали секунди след полунощ на първи януари 1970.
# Считаме, че файловете в тази директория, чиито имена нямат този формат, не са log файлове.

# Пример за име:
# correlator_1692117813.log

# Напишете серия от команди, намираща общия брой на срещанията на думата error във всички log
# файлове.

find /var/log/my_logs -type f | egrep "[a-zA-Z_]_[1-9][0-9]*\.log" | xargs -I{} cat {} | grep -ow "error" | wc -l
