#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T16:11:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graph
TEMPLATE = app
CONFIG += C++11

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

SOURCES += main.cpp\
        mainwindow.cpp \
    algorithm/algorithms.cpp \
    view/controls.cpp \
    structures/exceptions.cpp \
    structures/myspinbox.cpp \
    view/settings.cpp \
    graph/graph.cpp \
    structures/graphsearchworker.cpp \
    algorithm/getallvertexlistinordervorker.cpp \
    view/matrixitemmodel.cpp \
    algorithm/searchcyclepathworker.cpp \
    structures/graphitemselectsignalemitter.cpp \
    algorithm/getgraphvertexleavelistworker.cpp \
    graph/edge.cpp \
    graph/vertex.cpp \
    view/idselectdialog.cpp \
    structures/comparators.cpp \
    graph/graphicvertex.cpp \
    graph/graphicedge.cpp \
    structures/abstractvertex.cpp \
    structures/abstractedge.cpp \
    globalsettings.cpp \
    graphscene.cpp \
    graphview.cpp \
    view/graphdataview.cpp \
    view/pathsItemmodel.cpp \
    structures/directpushbutton.cpp \
    structures/colorpushbutton.cpp \
    util/rand.cpp

HEADERS  += mainwindow.h \
    structures/nodeData.h \
    algorithm/algorithms.h \
    view/controls.h \
    structures/exceptions.h \
    structures/myspinbox.h \
    view/settings.h \
    structures/graphsearchworker.h \
    algorithm/getallvertexlistinordervorker.h \
    view/matrixitemmodel.h \
    algorithm/searchcyclepathworker.h \
    structures/graphitemselectsignalemitter.h \
    algorithm/getgraphvertexleavelistworker.h \
    graph/graph.h \
    graph/edge.h \
    graph/vertex.h \
    view/pathsItemModel.h \
    view/idselectdialog.h \
    structures/graphTypes.h \
    structures/comparators.h \
    graph/graphicvertex.h \
    graph/graphicedge.h \
    structures/abstractvertex.h \
    structures/abstractedge.h \
    graphview.h \
    globalsettings.h \
    graphscene.h \
    view/graphdataview.h \
    structures/directpushbutton.h \
    structures/colorpushbutton.h \
    util/rand.h

FORMS    += mainwindow.ui \
    view/controls.ui \
    view/matrixview.ui \
    view/settings.ui \
    view/idselectdialog.ui
