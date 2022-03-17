#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
import sys

main_str = """#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
from PyQt5 import QtWidgets
from mainwindow import MainWindow

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    show = MainWindow()
    show.show()
    sys.exit(app.exec_())

"""

mainwindow_str = """#!/usr/bin/env python
# -*- coding:utf-8 -*-

from PyQt5 import QtCore, QtWidgets
from ui_mainwindow import Ui_MainWindow

class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

"""

ui_mainwindow_str = """<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>640</width>
    <height>480</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>MainWindow</string>
  </property>
  <widget class="QWidget" name="centralwidget"/>
  <widget class="QMenuBar" name="menubar">
   <property name="geometry">
    <rect>
     <x>0</x>
     <y>0</y>
     <width>640</width>
     <height>23</height>
    </rect>
   </property>
  </widget>
  <widget class="QStatusBar" name="statusbar"/>
 </widget>
 <resources/>
 <connections/>
</ui>

"""


if __name__ == '__main__':
    os.mknod("main.py")
    os.mknod("mainwindow.py")
    os.mknod("ui_mainwindow.ui")
    fp = open("main.py","w")
    fp.write(main_str)
    fp.close()
    fp = open("mainwindow.py","w")
    fp.write(mainwindow_str)
    fp.close()
    fp = open("ui_mainwindow.ui","w")
    fp.write(ui_mainwindow_str)
    fp.close()
    print("success")
