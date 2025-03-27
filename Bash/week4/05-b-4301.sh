# Напишете shell script, който автоматично да попълва файла указател от предната задача по подадени аргументи: име на файла указател, пълно име на човека (това, което очакваме да е в /etc/passwd) и избран за него nickname.
# Файлът указател нека да е във формат:
# <nickname, който лесно да запомните> <username в os-server>
# // може да сложите и друг delimiter вместо интервал

# Примерно извикване:
# ./pupulate_address_book myAddressBook "Ben Dover" uncleBen

# Добавя към myAddressBook entry-то:
# uncleBen <username на Ben Dover в os-server>

# ***Бонус: Ако има няколко съвпадения за въведеното име (напр. има 10 човека Ivan Petrov в /etc/passwd), всички те да се показват на потребителя, заедно с пореден номер >=1,
# cлед което той да може да въведе някой от номерата (или 0 ако не си хареса никого), и само избраният да бъде добавен към указателя.

 #!/bin/bash

 if [[ $# -ne 3 ]]; then
     echo "Expected three arguments"
     exit 1
 fi

 if [[ ! -f $1 ]]; then
      echo "First arg must be a file"
      exit 2
  fi
 
  filename="${1}"
  fullname="${2}"
  nickname="${3}"
 
  username=$(grep "${fullname}" /etc/passwd | awk -F ':' '{ print $1 }')
  usernamesCount=$(echo "${username}" | wc -l)
 
 
  if [[ $usernamesCount -gt 1 ]]; then
      index=1
 
      echo "0.Exit"
      while read line; do
          echo "${index}.${line}"
          index=$(( index + 1 ))
      done < <(echo "${username}")
 
      read -p "Choose from 1-${usernamesCount}: " choice
      if [[ ${choice} -lt 0 ]] || [[ ${choice} -gt ${usernamesCount} ]] ; then
          echo "Invalid choice"
          exit 1
      else
           if [[ ${choice} -eq 0 ]] ; then
              exit 0
           else
              username=$(echo "${username}" | head -n ${choice} | tail -n 1)
           fi
      fi
 
  fi
 
  echo "${nickname} ${username}" >> ${filename