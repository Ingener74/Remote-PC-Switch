#!/usr/bin/env python
# -*- coding: utf8 -*-
import sys
import json

from PySide2.QtWidgets import QApplication, QMessageBox

from MainWidget import MainWidget
from SerialPortThread import SerialPortThread

if __name__ == '__main__':
    app = QApplication(sys.argv)

    serial_port_thread = SerialPortThread()

    main_widget = MainWidget()

    main_widget.signal_port_select.connect(serial_port_thread.select_port)
    serial_port_thread.on_data.connect(main_widget.on_data_received)
    serial_port_thread.signal_connected.connect(main_widget.on_connected)
    serial_port_thread.signal_disconnected.connect(main_widget.on_disconnected)

    def on_serial_error(text):
        QMessageBox.critical(main_widget, 'Title', text)
    serial_port_thread.signal_error.connect(on_serial_error)

    def send_command(command):
        serial_port_thread.send_command(bytes(f'{json.dumps(command)}\n', 'utf-8'))

    def add_wifi(ssid, passwd):
        set_wifi = {
            "c": 1,
            "p": {
                "ssid": ssid,
                "pswd": passwd
            }
        }
        send_command(set_wifi)

    def show_settings():
        send_command({"c": 0})

    def load_settings():
        send_command({"c": 2})

    def start():
        serial_port_thread.start()

    def on_close():
        serial_port_thread.requestInterruption()
        serial_port_thread.wait()

    def on_test_query(query):
        send_command({
            'c': 4,
            'q': query
        })

    def on_query_test_timer(state):
        send_command({
            'c': 5,
            'e': state
        })

    def build_in_led_state(state):
        send_command({
            'c': 6,
            'e': state
        })

    main_widget.signal_test_query.connect(on_test_query)
    main_widget.signal_query_timer_ctrl.connect(on_query_test_timer)
    main_widget.signal_build_in_led.connect(build_in_led_state)

    main_widget.signal_on_close.connect(on_close)

    main_widget.signal_port_connect.connect(start)
    main_widget.add_wifi.connect(add_wifi)
    main_widget.show_settings.connect(show_settings)
    main_widget.load_settings.connect(load_settings)

    main_widget.show()

    sys.exit(app.exec_())
