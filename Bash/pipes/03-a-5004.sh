# Изведете целите редове от /etc/passwd за хората от 03-a-5003

cat /etc/passwd | cut -d ':' -f 5 | cut -d ',' -f1 | grep -E '.+ .{,7}$' | xargs -I {} grep -w {} /etc/passwd