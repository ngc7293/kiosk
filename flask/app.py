#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Micro Flask app to wrap kiosk's IPC responses in HTTP requests"""

from json import loads, JSONDecodeError
from socket import socket, AF_UNIX, SOCK_STREAM
from os import getenv

from flask import Flask, jsonify, request


sock = socket(family=AF_UNIX, type=SOCK_STREAM)
app = Flask(__name__)


@app.route('/', methods=['GET'])
def index():
    """Serve the Vue.JS single-page single-file app"""
    file = open('static/index.html')
    return file.read(), 200


@app.route('/', methods=['POST'])
def update():
    """Redirect HTTP to Unix Socket and vice-versa"""
    try:
        sock.send(request.data)
        data = loads(sock.recv(4096))
    except (BrokenPipeError, ConnectionError, OSError, JSONDecodeError):
        return jsonify({'status': 'error', 'message': 'server error'}), 500

    status = 200
    if 'status' in data and data['status'] == 'error':
        status = 400

    return jsonify(data), status


if __name__ == '__main__':
    sock.connect('/tmp/kiosk')
    app.run(host=getenv('FLASK_HOST', '127.0.0.1'))
