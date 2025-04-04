# Задачата ви е да напишете скрипт benchmark.sh, който измерва средното време за изпълнение на
# дадена команда. Първият аргумент на скрипта е число (време за провеждане на експеримента, в секунди), а останалите аргументи на скрипта са измерваната команда и нейните аргументи.
# Скриптът трябва да изпълнява подадената команда многократно, докато изтече подаденото време.
# Когато това се случи, скриптът трябва да изчака последното извикване на командата да приключи и да
# изведе съобщение, описващо броя направени извиквания, общото и средното време за изпълнение.
# $ ./benchmark.sh 60 convert image.jpg result.png
# Ran the command 'convert image.jpg result.png' 8 times for 63 seconds.
# Average runtime: 7.88 seconds.
# $ ./benchmark.sh 10 sleep 1.5
# Ran the command 'sleep 1.5' 7 times for 10.56 seconds.
# Average runtime: 1.51 seconds.
# Забележки:
# • Времената се извеждат в секунди, с точност два знака след запетайката.
# • Приемете, че времето на изпълнение на частите от скрипта извън подадената команда е пренабрежимо малко.