
QT       += core gui widgets

CONFIG += c++17

TARGET = GraphicPlotting
TEMPLATE = app

# 源文件
SOURCES +=     main.cpp     Shape.cpp     Circle.cpp     Rectangle.cpp     Triangle.cpp     ShapeManager.cpp     MainWindow.cpp     ShapeCanvas.cpp     AddShapeDialog.cpp

# 头文件
HEADERS +=     Shape.h     Circle.h     Rectangle.h     Triangle.h     ShapeManager.h     MainWindow.h     ShapeCanvas.h     AddShapeDialog.h
