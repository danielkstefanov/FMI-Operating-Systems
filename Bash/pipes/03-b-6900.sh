# Намерете имената на 10-те файла във вашата home директория, чието съдържание е
# редактирано най-скоро. На първо място трябва да бъде най-скоро редактираният
# файл.

find ~ -printf "%f %T@\n" | sort -t ' ' -k2 -r -n | head | cut -d ' ' -f1

#Намерете 10-те най-скоро достъпени файлове. (hint: Unix time)

find ~ -printf "%A@ %f\n" | sort -k 1nr -t ' ' | cut -d ' ' -f2 | head -n 10