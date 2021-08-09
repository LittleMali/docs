#codeing=utf-8

import socket
import time

ip = socket.gethostbyname(socket.gethostname())
port = 7781
print('ip=', ip)
print('port=', port)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('192.168.255.10', 0))
print('bind ', s.getsockname())
s.connect((ip, port))
print('connect sockname', s.getsockname())
print('connect peername', s.getpeername())

i = 0
while i < 10000:
    s.send(str('hello world: ' + str(i) + '\r\n').encode('utf-8'))
    #print(s.getpeername())
    i = i + 1
    time.sleep(1)

s.close()
    

