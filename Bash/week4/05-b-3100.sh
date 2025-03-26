#!/bin/bash

# Да се напише shell скрипт, който приканва потребителя да въведе низ
# - потребителско име на потребител от системата - след което извежда на
# стандартния изход колко активни сесии има потребителят в момента.

read -p "Please enter a username: " username

if ! grep -q "${username}" /etc/passwd ; then
        echo "No such username exists"
        exit 1
fi

active_sessions=$(who | awk '{print $1}' | grep -w "$username" | wc -l)
echo "The user '$username' has $active_sessions active sessions."