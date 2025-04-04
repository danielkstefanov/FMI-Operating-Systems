# При подреждане в нарастващ ред на числовите потребителски идентификатори (UID) на
# акаунтите, дефинирани в системата, 201-ят акаунт е от групата, запазена за акаунти от специалност СИ.

# Изведете списък с имената (име и фамилия) и home директориите на всички акаунти от специалност
# СИ, подреден по факултетен номер.

awk -v group=$(cat /etc/passwd | sort -t ':' -k3 -n | head -n 201 | tail -n 1 | cut -d ':' -f4) -F':' ' $4==group { sub(/,.*/, "", $5); printf "%s:%s \n",  $5 , $6 } ' /etc/passwd

# За справка:
# s61988:x:1219:504:Stoian Genchev,SI,2,5:/home/SI/s61988:/bin/bash
# s81430:x:1234:503:Iordan Petkov, KN, k2, g7:/home/KN/s81430:/bin/bash
# s61807:x:1248:504:Elica Venchova:/home/SI/s61807:/bin/bash
# s62009:x:1254:504:Denitsa Dobreva, 2, 6:/home/SI/s62009:/bin/bash
# s61756:x:1258:504:Katrin Kartuleva, SI, 4, 1:/home/SI/s61756:/bin/bash
# s855287:x:1195:504:Vaska Kichukova,SI,2,5:/home/SI/s855287:/bin/bash
# Примерен изход:
# Katrin Kartuleva:/home/SI/s61756
# Elica Venchova:/home/SI/s61807
# Stoian Genchev:/home/SI/s61988
# Denitsa Dobreva:/home/SI/s62009
# Vaska Kichukova:/home/SI/s855287