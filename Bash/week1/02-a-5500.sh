#Създайте файл, който да съдържа само последните 10 реда от изхода на 02-a-5403

find /etc -type d -mindepth 1 -maxdepth 1 | tail 1>last_ten_rows.txt