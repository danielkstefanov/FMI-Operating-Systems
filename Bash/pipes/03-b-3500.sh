#Колко файлове в /bin са 'shell script'-oве? 
ls -f /bin | xargs -I{} file "/bin/{}" | cut -d ',' -f1 | cut -d ':' -f2 |grep "shell script" | wc -l

#Колко файлове в дадена директория са ASCII text?
find /bin | xargs -I{} file {} | grep "ASCII text executable" | wc -l