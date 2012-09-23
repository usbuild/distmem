#!/usr/bin/env python
import socket  
import time  
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('localhost', 4327))  
while True:
    s = raw_input("Prompt:>");
    if s == "exit": break;
    s = s.lstrip()
    sa = s.split()
    p = []
    p.append(sa[0])
    if sa[0] == 'set':
        p.append(sa[1])
        p.append(s[s.find(" " + p[-1] +" ") + len(p[-1]) + 1:].lstrip())
    elif sa[0] == 'get':
        p.append(sa[1])
    else:
        print 'Unknown Command'
        continue
    request = "*" + str(len(p)) + "\r\n"
    for param in p:
        request += "$" + str(len(param)) +"\r\n"
        request += param + "\r\n"
    sock.send(request + "\n")
    print sock.recv(2048)
sock.close()  
