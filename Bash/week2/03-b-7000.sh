# Да приемем, че файловете, които съдържат C код, завършват на `.c` или `.h`.
# Колко на брой са те в директорията `/usr/include`?

find /usr/include -name '*.[ch]' |  wc -l

# Колко реда C код има в тези файлове?

cat $(find /usr/include -name '*.[ch]') |  wc -l