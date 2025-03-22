# Във file2 (inplace) подменете всички малки букви с главни.

cat ~/dir5/file22 | tr 'a-z' 'A-Z' > ~/dir5/file2
sed -i 's/[a-z]/\U&/g' ~/dir5/file2