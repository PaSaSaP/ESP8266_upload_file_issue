#!/usr/bin/env python3
import sys
import os
from socket import *
from time import sleep


valid_file = True
if len(sys.argv) > 1:
    valid_file = bool(eval(sys.argv[1]))
host = "192.168.55.180"
if len(sys.argv) > 2:
    host = sys.argv[2]


def create_inner_data(filename):
    result = "Content-Disposition: form-data; name=\"file\"; filename=\""+filename+"\"\r\n"
    result += "Content-Type: application/octet-stream\r\n\r\n"
    return result


def create_data(data, filename):
    border = b"----------border"
    inner_data = create_inner_data(filename).encode()
    content = F"--{border}\r\n".encode()
    content += inner_data
    if valid_file:
        content += data
        content += F"\r\n--{border}--\r\n".encode()
    else:
        content += "less data".encode()

    result = "POST /upload HTTP/1.1\r\n".encode()
    data_length = len(content)
    result += F"Content-Length: {data_length}\r\n".encode()
    result += F"Content-Type: multipart/form-data; boundary={border}\r\n\r\n".encode()
    result += content
    return result


def main():
    s = socket(AF_INET, SOCK_STREAM)
    s.settimeout(3)
    s.connect((host, 80))
    r = b'some data to be sent'
    if len(sys.argv) > 2:
        with open(sys.argv[2], 'rb') as f:
            r = f.read()
    file_name = 'valid.txt'
    if not valid_file:
        file_name = 'invalid.txt'
    d = create_data(r, file_name)
    s.send(d)
    if not valid_file:
        print("Disconnect WiFi now...")
    sleep(10)
    if valid_file:
        try:
            d=s.recv(1024)
            print(F"recv: {d}")
        except Exception as e:
            print(F"exception: {e}")
    s.close()


if __name__ == '__main__':
    main()
