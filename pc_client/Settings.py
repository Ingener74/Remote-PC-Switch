#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from PySide2.QtCore import QSettings
from Log import logger
settings = QSettings(QSettings.IniFormat, QSettings.UserScope, "ShnaiderPavel", "PC client")
logger.debug(settings.fileName())
