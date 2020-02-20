#!/usr/bin/env python3
import json

from socket import socket, AF_UNIX, SOCK_STREAM

from flask import Flask, Response, request

sock = socket(family=AF_UNIX, type=SOCK_STREAM)
sock.connect("/tmp/kiosk")

app = Flask(__name__)

@app.route('/', methods=['GET'])
def index():
    file = open("static/index.html")
    return Response(file.read(), 200, mimetype='text/html')

@app.route('/', methods=['POST'])
def update():
    sock.send(request.data)
    return Response(sock.recv(4096), 200, mimetype='application/json')

if __name__ == "__main__":
    app.run()