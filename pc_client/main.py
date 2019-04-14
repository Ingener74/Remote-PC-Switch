#!/usr/bin/env python
# -*- coding: utf8 -*-
import sys

import serial.tools.list_ports
from PySide2.QtCore import QThread, Slot, Signal
from PySide2.QtGui import QShowEvent
from PySide2.QtWidgets import QApplication, QWidget

from pc_client import Ui_MainWidget


class SerialPortThread(QThread):
    on_data = Signal(str)

    def __init__(self):
        super(SerialPortThread, self).__init__()
        self.device_path: str = ''

    def run(self):
        input_string = ''
        print(self.device_path)
        try:
            s = serial.Serial(self.device_path, 115200)
            while True:
                d = s.read()
                if d == b'\r' or d == b'\n':
                    input_string += d.decode('ascii')
                    self.on_data.emit(input_string)
                    input_string = ''
        except:
            print('Error')

    @Slot()
    def on_port_select(self, device_path: str):
        self.device_path = device_path


class MainWidget(QWidget):
    port_select = Signal(str)

    port_connect = Signal()

    def __init__(self, parent=None):
        super(MainWidget, self).__init__(parent=parent)
        self.ui = Ui_MainWidget()
        self.ui.setupUi(self)

        self.port: str = ''

        self.ui.pushButtonConnect.clicked.connect(self.on_connect_click)
        self.ui.listWidgetAvailablePorts.itemChanged.connect(self.on_port_select)
        self.ui.listWidgetAvailablePorts.itemClicked.connect(self.on_port_select)

    def showEvent(self, event: QShowEvent):
        super().showEvent(event)
        self.ui.listWidgetAvailablePorts.clear()
        for port in list(serial.tools.list_ports.comports()):
            self.ui.listWidgetAvailablePorts.addItem(port.device)

    def on_connect_click(self):
        print(f'Connect ')
        self.port_connect.emit()

    def on_port_select(self, item):
        self.port = item.text()
        print(f'Port selected: {self.port}')
        self.port_select.emit(self.port)

    @Slot()
    def on_data_received(self, data: str):
        self.ui.plainTextEditSerialLog.appendPlainText(data)


if __name__ == '__main__':
    app = QApplication(sys.argv)

    serial_port_thread = SerialPortThread()

    main_widget = MainWidget()

    main_widget.port_select.connect(serial_port_thread.on_port_select)
    serial_port_thread.on_data.connect(main_widget.on_data_received)


    def start():
        serial_port_thread.start()


    main_widget.port_connect.connect(start)

    main_widget.show()

    sys.exit(app.exec_())
