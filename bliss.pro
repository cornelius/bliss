TEMPLATE = app
TARGET = bliss

target.path = /usr/bin
INSTALLS += target

INCLUDEPATH += . minikde lib
QT += xml gui widgets

# Input
HEADERS += src/blissallitemmodel.h \
           src/blissitemmodel.h \
           src/blisstodoitemmodel.h \
           src/buttonitem.h \
           src/delayedsignal.h \
           src/droptargetitem.h \
           src/fanmenu.h \
           src/fanmenuelement.h \
           src/fanmenuitem.h \
           src/groupadderitem.h \
           src/groupaddersidebaritem.h \
           src/groupview.h \
           src/hidinglineedit.h \
           src/history.h \
           src/historyview.h \
           src/itemplacer.h \
           src/listitem.h \
           src/magicmenuitem.h \
           src/mainmenuitem.h \
           src/mainmodel.h \
           src/mainview.h \
           src/mainwindow.h \
           src/matchlist.h \
           src/menuhandleitem.h \
           src/menuhandler.h \
           src/newgroupdialog.h \
           src/newlistdialog.h \
           src/newtododialog.h \
           src/overview.h \
           src/roundedrectitem.h \
           src/searchedit.h \
           src/searchresultview.h \
           src/settingswidget.h \
           src/storage.h \
           src/storagefile.h \
           src/storagegit.h \
           src/todohandleitem.h \
           src/todoitem.h \
           src/trackinggraphicsview.h \
           lib/bliss/bliss.h \
           lib/bliss/bliss_export.h \
           lib/gitdata/command.h \
           lib/gitdata/dir.h \
           lib/gitdata/gitdata_export.h \
           lib/gitdata/remote.h
SOURCES += src/blissallitemmodel.cpp \
           src/blissitemmodel.cpp \
           src/blisstodoitemmodel.cpp \
           src/buttonitem.cpp \
           src/delayedsignal.cpp \
           src/droptargetitem.cpp \
           src/fanmenu.cpp \
           src/fanmenuelement.cpp \
           src/fanmenuitem.cpp \
           src/groupadderitem.cpp \
           src/groupaddersidebaritem.cpp \
           src/groupview.cpp \
           src/hidinglineedit.cpp \
           src/history.cpp \
           src/historyview.cpp \
           src/itemplacer.cpp \
           src/listitem.cpp \
           src/magicmenuitem.cpp \
           src/main.cpp \
           src/mainmenuitem.cpp \
           src/mainmodel.cpp \
           src/mainview.cpp \
           src/mainwindow.cpp \
           src/matchlist.cpp \
           src/menuhandleitem.cpp \
           src/menuhandler.cpp \
           src/newgroupdialog.cpp \
           src/newlistdialog.cpp \
           src/newtododialog.cpp \
           src/overview.cpp \
           src/roundedrectitem.cpp \
           src/searchedit.cpp \
           src/searchresultview.cpp \
           src/settings.cpp \
           src/settingswidget.cpp \
           src/storage.cpp \
           src/storagefile.cpp \
           src/storagegit.cpp \
           src/todohandleitem.cpp \
           src/todoitem.cpp \
           src/trackinggraphicsview.cpp \
           lib/bliss/bliss.cpp \
           lib/gitdata/command.cpp \
           lib/gitdata/dir.cpp \
           lib/gitdata/remote.cpp \
           minikde/klocale.cpp \
           minikde/kdialog.cpp \
           minikde/krandom.cpp
