#codeing=utf-8

import socket
import sys

hostname = socket.gethostname()
ip = socket.gethostbyname(hostname)
port = 7781
print('hostname= ', hostname)
print('ip=', ip)
print('port=', port)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_IP)
s.bind(('0.0.0.0', port))
s.listen()

while True:
    conn, addr = s.accept()
    print('connect has reached: ', addr)
    print('getsockname: ', conn.getsockname())
    print('getpeername: ', conn.getpeername())

    while True:
        data = conn.recv(1024)
        if len(data) == 0:
            print('client has close')
            break
        print('recv client: ', data)

    conn.close()
    print("client conn has closed")

    quit = input('quit ? ')
    if quit == 'y':
        print('select quit')
        break

s.close()
print('tcp srv has closed')
