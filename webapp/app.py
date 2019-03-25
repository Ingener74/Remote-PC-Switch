#!/usr/bin/env python
# -*- coding: utf8 -*-

from threading import Timer

from flask import Flask, jsonify, render_template

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

esp8266_timer = None


def start_timer():
    global esp8266_timer
    esp8266_timer = Timer(10, esp8266_timeout)
    esp8266_timer.start()


def esp8266_timeout():
    clients['esp8266']['status'] = 'OFFLINE'
    start_timer()


@app.route('/')
def index():
    return render_template('index.html', clients=clients)


@app.route('/esp8266_status')
def esp8266_status():
    return jsonify({
        'status': clients['esp8266']['status']
    })


@app.route('/esp8266_poweron')
def esp8266_poweron():
    print("PowerOn")
    clients['client']['act'] = 'POWER_ON'
    return jsonify({})


@app.route('/esp8266_power_on_reset')
def esp8266_poweroff():
    print("PowerOff")
    clients['client']['act'] = 'POWER_ON_RESET'
    return jsonify({})


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


# @app.route('/client/<act>')
# def client(act):
#     if act == 'ON':
#         clients['client']['act'] = 'SWITCH_ON'
#     else:
#         clients['client']['act'] = 'DO_NOTHING'

start_timer()

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
