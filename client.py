#!/usr/bin/env python
import socket  
import time  
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('localhost', 4327))  
param1 = []
param2 = ['use', 'get']
param3 = ['set']
while True:
    s = raw_input("Prompt:>");
    if s == "exit": break;
    s = s.lstrip()
    sa = s.split()
    p = []
    p.append(sa[0])
    if sa[0] in param1:
        pass
    elif sa[0] in param2:
        p.append(sa[1])
    elif sa[0] in param3:
        p.append(sa[1])
        p.append(s[s.find(" " + p[-1] +" ") + len(p[-1]) + 1:].lstrip())
        try:
            exec("data = " + p[-1]);#to assign data to the value
        except:
            print 'Syntax Error'
            continue
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
