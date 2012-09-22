#!/usr/bin/env python
import socket  
import time  
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('localhost', 4327))  
while True:
    s = raw_input("Prompt:>");
    if s == "exit": break;
    sock.send(s + "\n")
    print sock.recv(2048)  
sock.close()  
