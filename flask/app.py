#!/usr/bin/env python3
import json

from socket import socket, AF_UNIX, SOCK_STREAM

from flask import Flask, Response, request


def connect():
    sock.connect("/tmp/kiosk")


sock = socket(family=AF_UNIX, type=SOCK_STREAM)
app = Flask(__name__)


@app.route('/', methods=['GET'])
def index():
    file = open("static/index.html")
    return Response(file.read(), 200, mimetype='text/html')

@app.route('/', methods=['POST'])
def update():
    try:
        sock.send(request.data)
    except BrokenPipeError:
        return Response(json.dumps({'status': 'error', 'message': 'broken pipe'}), 500, mimetype='application/json')

    data = json.loads(sock.recv(4096))

    status = 200
    if 'status' in data and data['status'] == 'error':
        status = 400

    print(data)

    return Response(json.dumps(data), status=status, mimetype='application/json')


if __name__ == "__main__":
    connect()
    app.run(host='192.168.8.68')
