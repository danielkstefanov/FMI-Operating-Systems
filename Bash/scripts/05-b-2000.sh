#!/bin/bash
# Да се напише shell скрипт, който приканва потребителя да въведе низ (име) и изпечатва "Hello, низ".

read -p "Please enter your name: "name
echo "Hello, ${name}"