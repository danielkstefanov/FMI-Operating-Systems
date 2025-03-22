# За всяка група от /etc/group изпишете "Hello, <група>", като ако това е вашата група, напишете "Hello, <група> - I am here!".

cat /etc/group | cut -d ':' -f 1 | awk -v user_group=$(id -gn) '$1 == user_group { print "Hello " $1 " - I am here!" } $1 != user_group { print "Hello " $1 }'