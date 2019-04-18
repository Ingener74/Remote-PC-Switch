#!/usr/bin/env python
# -*- coding: utf8 -*-

from threading import Timer
import logging

from flask import Flask, jsonify, render_template, request

# Убрать 

app = Flask(__name__, static_folder='static', template_folder='static')

clients = {
    'client': {
        'act': 'DO_NOTHING'
    },
    'esp8266': {
        'status': 'OFFLINE',
        'timer': None
    }
}

last_commands = []

esp8266_timer = None


def start_timer():
    global esp8266_timer
    esp8266_timer = Timer(10, esp8266_timeout)
    esp8266_timer.start()


def esp8266_timeout():
    clients['esp8266']['status'] = 'OFFLINE'
    start_timer()


@app.before_first_request
def setup_logging():
    if not app.debug:
        app.logger.addHandler(logging.StreamHandler())
        app.logger.setLevel(logging.INFO)


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.is_json:
        request_get_json = request.get_json()
        if request_get_json['command'] == 'status':
            return jsonify({
                'status': clients['esp8266']['status'],
                'act': clients['client']['act']
            })
        elif request_get_json['command'] == 'esp8266_power_on':
            app.logger.info("PowerOn")
            clients['client']['act'] = 'POWER_ON'
            return jsonify({})
        elif request_get_json['command'] == 'esp8266_power_on_reset':
            app.logger.info("PowerOnReset")
            clients['client']['act'] = 'POWER_ON_RESET'
            return jsonify({})
        else:
            app.logger.error(f'unknown command: {request_get_json}')
            return jsonify(request_get_json)
    else:
        return render_template('index.html', clients=clients)


@app.route('/esp8266/<act>')
def client(act):
    if act == 'QUERY':
        global esp8266_timer
        esp8266_timer.cancel()
        start_timer()

        clients['esp8266']['status'] = 'ONLINE'

        return jsonify({
            'act': clients['client']['act']
        })
    elif act == 'STATUS':
        return jsonify({
            'status': 'test'
        })


start_timer()

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
