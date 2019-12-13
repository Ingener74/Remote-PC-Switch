#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import traceback
from serial.tools.list_ports import comports
from PySide2.QtWidgets import QWidget
from PySide2.QtGui import QCloseEvent, QShowEvent, QKeyEvent
from PySide2.QtCore import Qt, Slot, Signal

from Settings import settings
from Ui_MainWidget import Ui_MainWidget
from Log import logger

class MainWidget(QWidget):
    GEOM = 'mw_geometry'
    CURRENT_PORT = 'current_port'

    signal_port_select = Signal(str)

    signal_port_connect = Signal()

    add_wifi = Signal(str, str)
    show_settings = Signal()
    load_settings = Signal()
    
    signal_on_close = Signal()
    signal_test_query = Signal(str)
    signal_query_timer_ctrl = Signal(bool)
    signal_build_in_led = Signal(bool)

    def __init__(self, parent=None):
        super(MainWidget, self).__init__(parent)
        self.ui = Ui_MainWidget()
        self.ui.setupUi(self)

        self.settings = settings

        self.port: str = ''

        self.ui.pushButtonConnect.clicked.connect(self.on_connect_click)
        self.ui.listWidgetAvailablePorts.itemChanged.connect(self.on_port_select)
        self.ui.listWidgetAvailablePorts.itemClicked.connect(self.on_port_select)
        self.ui.listWidgetAvailablePorts.currentRowChanged.connect(self.on_port_select_row)
        self.ui.pushButtonAddWiFiAP.clicked.connect(self.on_add_wifi_click)

        self.ui.pushButtonShowSettings.clicked.connect(self.on_show_settings)
        self.ui.pushButtonLoadSettings.clicked.connect(self.on_load_settings_click)
        self.ui.pushButtonTestQuery.clicked.connect(self.on_test_query_click)

        self.ui.checkBoxCtrlQueryTimer.stateChanged.connect(self.on_query_timer_ctrl_state_changed)
        self.ui.checkBoxBuildInLed.stateChanged.connect(self.on_build_in_led_changed_state)

        self.ui.lineEditTestQuery.setEnabled(False)
        self.ui.pushButtonTestQuery.setEnabled(False)

    def showEvent(self, event: QShowEvent):
        super().showEvent(event)
        self.ui.listWidgetAvailablePorts.clear()
        for port in list(comports()):
            self.ui.listWidgetAvailablePorts.addItem(port.device)
        
        if self.settings.contains(self.CURRENT_PORT):
            port = self.settings.value(self.CURRENT_PORT)
            self.ui.listWidgetAvailablePorts.setCurrentRow(int(port))

        if self.settings.contains(self.GEOM):
            self.setGeometry(self.settings.value(self.GEOM))

    def closeEvent(self, event: QCloseEvent):
        super().closeEvent(event)
        self.settings.setValue(self.GEOM, self.geometry())
        self.settings.setValue(self.CURRENT_PORT, self.ui.listWidgetAvailablePorts.currentRow())
        self.signal_on_close.emit()

    def keyPressEvent(self, event: QKeyEvent):
        if event.key() == Qt.Key_Escape:
            self.close()
        else:
            event.ignore()

    def on_connect_click(self):
        self.signal_port_connect.emit()

    def on_connected(self):
        self.ui.pushButtonConnect.setEnabled(False)

    def on_disconnected(self):
        self.ui.pushButtonConnect.setEnabled(True)

    def on_port_select_row(self, row):
        self.port = self.ui.listWidgetAvailablePorts.item(row).text()
        self.signal_port_select.emit(self.port)

    def on_port_select(self, item):
        self.port = item.text()
        self.signal_port_select.emit(self.port)

    def on_add_wifi_click(self):
        self.add_wifi.emit(self.ui.lineEditSSID.text(), self.ui.lineEditAPPassword.text())

    def on_show_settings(self):
        self.show_settings.emit()

    def on_load_settings_click(self):
        self.load_settings.emit()

    def on_data_received(self, data: str):
        js = None
        try:
            js = json.loads(data)
        except Exception as e:
            logger.error(f'Error: {e} for message {data}, {traceback.format_exc()}')
            
        if js is not None:
            json_text = json.dumps(js, separators=(', ', ':'), indent=4)
            self.ui.plainTextEditSerialLog.setPlainText(json_text)

            if 'wifier' in js:
                self.ui.checkBoxWiFierState.setChecked(js['wifier'] == 'connnected')
                self.ui.lineEditTestQuery.setEnabled(js['wifier'] == 'connnected')
                self.ui.pushButtonTestQuery.setEnabled(js['wifier'] == 'connnected')

                if 'ip' in js:
                    self.ui.lineEditWiFierIp.setText(js['ip'])
                else:
                    self.ui.lineEditWiFierIp.setText('')
        else:
            logger.info(data)

    def on_test_query_click(self):
        self.signal_test_query.emit(self.ui.lineEditTestQuery.text())

    def on_query_timer_ctrl_state_changed(self, state):
        self.signal_query_timer_ctrl.emit(state)

    def on_build_in_led_changed_state(self, state):
        self.signal_build_in_led.emit(state)
