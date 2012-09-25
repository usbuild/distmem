#!/usr/bin/env python
# -*- coding:utf-8 -*-
import socket  
import time  
import re
r = re.compile(r'(?<!\\),|(?<=\\\\),')
def getFlag(var):
    d = {'float':'f', 'int':'i', 'str':'s', 'list':'l'}
    if d.has_key(type(var).__name__):
        return d[type(var).__name__]
    else:
        return None

def parseCMD(data):
        if data[0] == "s":
            raw_data = data[1:]
        elif data[0] == "f":
            raw_data = float(data[1:])
        elif data[0] == "i":
            raw_data = int(data[1:])
        elif data[0] == "l":
            raw_array = r.split(data[1:])
            raw_data = []
            for item in raw_array:
                if item[0] == "s":
                    item = item.replace('\,', ',').replace('\\\\', '\\')
                raw_data.append(parseCMD(item));
        else:
            raw_data = data
        return raw_data
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
    data = None
    if sa[0] in param1:
        pass
    elif sa[0] in param2:
        p.append(sa[1])
    elif sa[0] in param3:
        p.append(sa[1])
        p.append(s[s.find(" " + p[-1] +" ") + len(p[-1]) + 1:].lstrip())
        try:
            exec("data = " + p[-1]);#to assign data to the value
            p[-1] = data
        except:
            print 'Syntax Error'
            continue
    else:
        print 'Unknown Command'
        continue

    request = "*" + str(len(p)) + "\r\n"
    if data is None:
        for param in p:
            request += "$" + str(len(param)) +"\r\n"
            request += param + "\r\n"
    else:
        for param in p[:-1]:
            request += "$" + str(len(param)) +"\r\n"
            request += param + "\r\n"
        
        if type(data).__name__ in ('int', 'float', 'str'):
            request += "$" + str(len(str(data)) + 1) + "\r\n"
            request += getFlag(data) + str(data) + "\r\n"
        elif type(data) is list:
            #list类型以,为分隔符，对于字符中出现的,应使用\,转义
            data_arr = []
            for d in data:
                symbol = getFlag(d)
                if d is None:
                    print 'Unknown Value'
                    continue
                data_arr.append(symbol + str(d).replace('\\', '\\\\').replace(',', '\,'))
            data_str = ','.join(data_arr)
            request += "$" + str(len(data_str) + 1) + "\r\n"
            request += getFlag(data) + data_str + "\r\n"
        else:
            print 'Unknown Value'
            continue

    sock.send(request + "\n")
    time.sleep(0.05)
    re = sock.recv(2048)
    if re[0] in ('+', '-'):
        print re[1:]
    elif re[0] == '$':
        resp_len = int(re[1:re.find("\r\n")])
        if resp_len == -1:
            print "Not Found"
            continue
        data_start = re.find("\r\n") + 2;
        data = re[data_start:data_start + resp_len]

        print parseCMD(data)
sock.close()
