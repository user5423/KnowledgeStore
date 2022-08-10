#!/bin/bash

if [[ ! -f key.pem ]] || [[ ! -f cert.pem ]]
then
	sudo openssl req -newkey rsa:2048 -new -nodes -x509 -days 365 -keyout key.pem -out cert.pem
fi

sudo openssl s_server -tls1_$1 -accept 443 -key key.pem -cert cert.pem
