#!/usr/bin/env python
# -*- coding: utf8 -*-
import json
import sys
import traceback
from json import JSONDecodeError
from typing import List

import PySide2
import serial.tools.list_ports
from PySide2.QtCore import QThread, Slot, Signal, QModelIndex, QMutex, QSettings
from PySide2.QtGui import QShowEvent
from PySide2.QtWidgets import QApplication, QWidget
from serial import SerialException

from Ui_MainWidget import Ui_MainWidget

class SerialPortThread(QThread):
    on_data = Signal(str)
    on_error = Signal(str)
    on_stop = Signal()

    def __init__(self):
        super(SerialPortThread, self).__init__()
        self.device_path: str = ''

        self.commands_mutex = QMutex()
        self.commands: List[bytes] = []

    def run(self):
        print(self.device_path)
        try:
            # check port permissions
            # https://stackoverflow.com/questions/1861836/checking-file-permissions-in-linux-with-python
            # https://stackoverflow.com/questions/539133/python-test-directory-permissions
            s = serial.Serial(self.device_path, 115200, timeout=0.1)
            while s.isOpen():
                d = s.readline()
                try:
                    if d is not bytes():
                        print(d)
                        self.on_data.emit(d.decode('ascii'))

                    self.__send_command(s)
                except:
                    print('Error')
        except (SerialException) as e:
            self.on_error.emit(f'Serial error {e}')
        except (UnicodeDecodeError) as e:
            self.on_error.emit(f'Unicode error {e}, {traceback.format_exc()}')
        self.on_stop.emit()

    def __send_command(self, serial_port):
        self.commands_mutex.lock()
        if len(self.commands) > 0:
            command = self.commands[0]
            rest_commands = self.commands[1:]

            print(f'send command:  {command}')
            print(f'rest commands: {rest_commands}')

            self.commands = rest_commands
            print(f'1')
            serial_port.write(command)
            print(f'2')
        self.commands_mutex.unlock()

    @Slot()
    def abort(self):
        pass

    @Slot()
    def select_port(self, device_path: str):
        self.device_path = device_path

    @Slot()
    def send_command(self, command: bytes):
        self.commands_mutex.lock()
        print(f'Send command: {command}')
        self.commands += [command]
        self.commands_mutex.unlock()


class MainWidget(QWidget):
    port_select = Signal(str)

    port_connect = Signal()

    add_wifi = Signal(str, str)
    show_settings = Signal()

    load_settings = Signal()

    def __init__(self, parent=None):
        super(MainWidget, self).__init__(parent)
        self.ui = Ui_MainWidget()
        self.ui.setupUi(self)

        self.settings = QSettings()

        self.port: str = ''

        self.ui.pushButtonConnect.clicked.connect(self.on_connect_click)
        self.ui.listWidgetAvailablePorts.itemChanged.connect(self.on_port_select)
        self.ui.listWidgetAvailablePorts.itemClicked.connect(self.on_port_select)
        self.ui.pushButtonAddWiFiAP.clicked.connect(self.on_add_wifi_click)

        self.ui.pushButtonShowSettings.clicked.connect(self.on_show_settings)
        self.ui.pushButtonLoadSettings.clicked.connect(self.on_load_settings_click)

    def showEvent(self, event: QShowEvent):
        super().showEvent(event)
        self.ui.listWidgetAvailablePorts.clear()
        for port in list(serial.tools.list_ports.comports()):
            self.ui.listWidgetAvailablePorts.addItem(port.device)
        # if self.ui.listWidgetAvailablePorts.count() > 0:
        #     self.ui.listWidgetAvailablePorts.setCurrentRow(0)

        if 'mwg' in self.settings.allKeys():
            self.setGeometry(self.settings.value('mwg'))

    def closeEvent(self, event: PySide2.QtGui.QCloseEvent):
        super().closeEvent(event)
        self.settings.setValue('mwg', self.geometry())

    def on_connect_click(self):
        self.port_connect.emit()

    def on_port_select(self, item):
        self.port = item.text()
        self.port_select.emit(self.port)

    def on_add_wifi_click(self):
        self.add_wifi.emit(self.ui.lineEditSSID.text(), self.ui.lineEditAPPassword.text())

    def on_show_settings(self):
        self.show_settings.emit()

    def on_load_settings_click(self):
        self.load_settings.emit()

    @Slot()
    def on_data_received(self, data: str):
        js = None
        try:
            js = json.loads(data)
        except JSONDecodeError as e:
            print(f'Error: {e} for message {data}')
            # self.ui.plainTextEditSerialLog.setPlainText(f'Error: {e} for message {data}')
        if js is not None:
            json_text = json.dumps(js, separators=(', ', ':'), indent=4)
            # print(json_text)
            self.ui.plainTextEditSerialLog.setPlainText(json_text)
        else:
            # self.ui.plainTextEditSerialLog.setPlainText(data)
            print(data)


if __name__ == '__main__':
    app = QApplication(sys.argv)

    serial_port_thread = SerialPortThread()

    main_widget = MainWidget()

    main_widget.port_select.connect(serial_port_thread.select_port)
    serial_port_thread.on_data.connect(main_widget.on_data_received)

    # def on_serial_thread_stop():
    #     QMess

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


    main_widget.port_connect.connect(start)
    main_widget.add_wifi.connect(add_wifi)
    main_widget.show_settings.connect(show_settings)
    main_widget.load_settings.connect(load_settings)

    main_widget.show()

    sys.exit(app.exec_())
