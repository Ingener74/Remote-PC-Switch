#!/usr/bin/env python
# -*- coding: utf8 -*-
import sys

from PySide2.QtWidgets import QApplication, QWidget


class MainWidget(QWidget):
    def __init__(self, parent=None):
        super(MainWidget, self).__init__(parent=parent)


if __name__ == '__main__':
    app = QApplication(sys.argv)

    main_widget = MainWidget()
    main_widget.show()

    sys.exit(app.exec_())
