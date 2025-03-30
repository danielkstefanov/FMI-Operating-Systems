# Да се напише shell скрипт, който получава единствен аргумент директория
# и отпечатва списък с всички файлове и директории в нея (без скритите).
# До името на всеки файл да седи размера му в байтове, а до името на всяка
# директория да седи броят на елементите в нея (общ брой на файловете и директориите, без скритите).

# a) Добавете параметър -a, който указва на скрипта да проверява и скритите файлове и директории.


 #!/bin/bash

 if [[ $# -gt 2 || $# -lt 1 ]]; then
     echo "Expected first arg: directory"
     echo "Optional second arg: -a"
     exit 1
 fi

 showHidden=0
 
  if [[ $# -eq 2 && $2 != "-a" ]]; then
      echo "Invalid second argument"
      exit 2
  else
      showHidden=1
  fi
 
 
  if [[ ! -d "${1}" ]]; then
      echo "Invalid first argument: expected dir"
      exit 2
  fi
 
  objects=$(ls ${2} ${1})
 
  for obj in ${objects}; do
      if [[ -f "${obj}" ]]; then
          echo  "${obj} ($(wc -c $obj | cut -d ' ' -f 1 ) bytes)"
      fi
 
      if [[ -d "${obj}" ]]; then
          echo "${obj} ($(ls ${2} ${obj} | wc -l) entries)"
      fi
  done



# Пример:
# $ ./list.sh .
# asdf.txt (250 bytes)
# Documents (15 entries)
# empty (0 entries)
# junk (1 entry)
# karh-pishtov.txt (8995979 bytes)
# scripts (10 entries)