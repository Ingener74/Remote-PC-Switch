# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'res/MainWidget.ui',
# licensing of 'res/MainWidget.ui' applies.
#
# Created: Sun Apr  7 15:51:41 2019
#      by: pyside2-uic  running on PySide2 5.12.2
#
# WARNING! All changes made in this file will be lost!

from PySide2 import QtCore, QtGui, QtWidgets

class Ui_MainWidget(object):
    def setupUi(self, MainWidget):
        MainWidget.setObjectName("MainWidget")
        MainWidget.resize(550, 286)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/chip.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWidget.setWindowIcon(icon)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(MainWidget)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.groupBox_serial = QtWidgets.QGroupBox(MainWidget)
        self.groupBox_serial.setObjectName("groupBox_serial")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.groupBox_serial)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.listWidgetAvailablePorts = QtWidgets.QListWidget(self.groupBox_serial)
        self.listWidgetAvailablePorts.setObjectName("listWidgetAvailablePorts")
        self.verticalLayout_2.addWidget(self.listWidgetAvailablePorts)
        self.pushButtonConnect = QtWidgets.QPushButton(self.groupBox_serial)
        self.pushButtonConnect.setObjectName("pushButtonConnect")
        self.verticalLayout_2.addWidget(self.pushButtonConnect)
        self.plainTextEditSerialLog = QtWidgets.QPlainTextEdit(self.groupBox_serial)
        self.plainTextEditSerialLog.setObjectName("plainTextEditSerialLog")
        self.verticalLayout_2.addWidget(self.plainTextEditSerialLog)
        self.horizontalLayout_2.addWidget(self.groupBox_serial)
        self.verticalLayout_3 = QtWidgets.QVBoxLayout()
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.groupBox_wifi = QtWidgets.QGroupBox(MainWidget)
        self.groupBox_wifi.setObjectName("groupBox_wifi")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.groupBox_wifi)
        self.verticalLayout.setObjectName("verticalLayout")
        self.formLayout = QtWidgets.QFormLayout()
        self.formLayout.setObjectName("formLayout")
        self.label = QtWidgets.QLabel(self.groupBox_wifi)
        self.label.setObjectName("label")
        self.formLayout.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label)
        self.lineEdit = QtWidgets.QLineEdit(self.groupBox_wifi)
        self.lineEdit.setEchoMode(QtWidgets.QLineEdit.Normal)
        self.lineEdit.setObjectName("lineEdit")
        self.formLayout.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.lineEdit)
        self.label_2 = QtWidgets.QLabel(self.groupBox_wifi)
        self.label_2.setToolTip("")
        self.label_2.setObjectName("label_2")
        self.formLayout.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_2)
        self.lineEdit_2 = QtWidgets.QLineEdit(self.groupBox_wifi)
        self.lineEdit_2.setEchoMode(QtWidgets.QLineEdit.Password)
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.formLayout.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.lineEdit_2)
        self.verticalLayout.addLayout(self.formLayout)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.pushButtonAddWiFiAP = QtWidgets.QPushButton(self.groupBox_wifi)
        self.pushButtonAddWiFiAP.setObjectName("pushButtonAddWiFiAP")
        self.horizontalLayout.addWidget(self.pushButtonAddWiFiAP)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_3.addWidget(self.groupBox_wifi)
        spacerItem1 = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_3.addItem(spacerItem1)
        self.horizontalLayout_2.addLayout(self.verticalLayout_3)

        self.retranslateUi(MainWidget)
        QtCore.QMetaObject.connectSlotsByName(MainWidget)

    def retranslateUi(self, MainWidget):
        MainWidget.setWindowTitle(QtWidgets.QApplication.translate("MainWidget", "PC Client", None, -1))
        self.groupBox_serial.setTitle(QtWidgets.QApplication.translate("MainWidget", "SerialPort", None, -1))
        self.pushButtonConnect.setText(QtWidgets.QApplication.translate("MainWidget", "Connect", None, -1))
        self.groupBox_wifi.setTitle(QtWidgets.QApplication.translate("MainWidget", "Add WiFi AccessPoint", None, -1))
        self.label.setText(QtWidgets.QApplication.translate("MainWidget", "SSID", None, -1))
        self.lineEdit.setPlaceholderText(QtWidgets.QApplication.translate("MainWidget", "Enter Access Point SSID", None, -1))
        self.label_2.setText(QtWidgets.QApplication.translate("MainWidget", "Password", None, -1))
        self.lineEdit_2.setPlaceholderText(QtWidgets.QApplication.translate("MainWidget", "Enter Access Point password", None, -1))
        self.pushButtonAddWiFiAP.setText(QtWidgets.QApplication.translate("MainWidget", "Add Access Point", None, -1))

import resources_rc
