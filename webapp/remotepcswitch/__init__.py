#!/usr/bin/env python
# -*- coding: utf8 -*-

from flask import Flask, jsonify


def create_app():
    app = Flask(__name__)

    @app.route('/')
    def index():
        return jsonify({
            'status': 'ok'
        })

    return app
