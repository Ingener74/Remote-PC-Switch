#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import traceback
import serial
from Log import logger
from PySide2.QtCore import QThread, Slot, Signal, QMutex

class SerialPortThread(QThread):
    on_data = Signal(str)
    signal_error = Signal(str)
    signal_connected = Signal()
    signal_disconnected = Signal()

    def __init__(self):
        super(SerialPortThread, self).__init__()
        self.device_path: str = ''

        self.commands_mutex = QMutex()
        self.commands: List[bytes] = []

    def run(self):
        logger.debug(self.device_path)
        try:
            s = serial.Serial(self.device_path, 115200, timeout=0.05)
            if s.isOpen():
                self.signal_connected.emit()
            while s.isOpen():
                d = s.readline()
                # logger.debug("I'm working")
                try:
                    if d is not bytes():
                        logger.debug(d)
                        self.on_data.emit(d.decode('ascii'))

                    self.__send_command(s)
                except Exception as e:
                    logger.error(f'Error: {e}, {traceback.format_exc()}')
                if self.isInterruptionRequested():
                    logger.debug('Interruption requested')
                    break
        except Exception as e:
            s = f'Error: {e}, {traceback.format_exc()}'
            logger.error(s)
            self.signal_error.emit(s)
        self.signal_disconnected.emit()

    def __send_command(self, serial_port):
        self.commands_mutex.lock()
        if len(self.commands) > 0:
            command = self.commands[0]
            self.commands = self.commands[1:]
            serial_port.write(command)
        self.commands_mutex.unlock()

    def select_port(self, device_path: str):
        self.device_path = device_path

    def send_command(self, command: bytes):
        self.commands_mutex.lock()
        logger.debug(f'Send command: {command}')
        self.commands += [command]
        self.commands_mutex.unlock()

