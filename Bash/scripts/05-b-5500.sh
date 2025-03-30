# Да се напише shell script, който генерира HTML таблица съдържаща описание на
# потребителите във виртуалката ви. Таблицата трябва да има:
# - заглавен ред с имената нa колоните
# - колони за username, group, login shell, GECOS field (https://en.wikipedia.org/wiki/Gecos_field)

# Пример:
# $ ./passwd-to-html.sh > table.html
# $ cat table.html
# <table>
#   <tr>
#     <th>Username</th>
#     <th>group</th>
#     <th>login shell</th>
#     <th>GECOS</th>
#   </tr>
#   <tr>
#     <td>root</td>
#     <td>root</td>
#     <td>/bin/bash</td>
#     <td>GECOS here</td>
#   </tr>
#   <tr>
#     <td>ubuntu</td>
#     <td>ubuntu</td>
#     <td>/bin/dash</td>
#     <td>GECOS 2</td>
#   </tr>   
# </table>



#!/bin/bash
printf "<table>\n";
printf "  <tr>\n";
printf "    <th>Username</th>\n";
printf "    <th>group</th>\n";
printf "    <th>login shell</th>\n";
printf "    <th>Gecos</th>\n";
printf "  </tr>\n";

while read line; do

    username=$(echo "$line" | cut -d ':' -f1)
    group=$(echo "$line" | cut -d ':' -f3 | xargs -I{} id -ng {})
    loginshell=$(echo "$line" | cut -d ':' -f7)
    gecos=$(echo "$line" | cut -d ':' -f5 | cut -d ',' -f 1)

     printf "  <tr>\n";
     printf "    <th>${username}</th>\n";
     printf "    <th>${group}</th>\n";
     printf "    <th>${loginshell}</th>\n";
     printf "    <th>${gecos}</th>\n";
     printf "  </tr>\n";

done < <(cat /etc/passwd)

printf "</table>\n";
