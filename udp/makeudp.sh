#!/bin/bash
g++ -g  -I ./ -c ./UdpSocket.cpp -o Udp.o
g++ -g  -lpthread  Udp.o  -o Udp
