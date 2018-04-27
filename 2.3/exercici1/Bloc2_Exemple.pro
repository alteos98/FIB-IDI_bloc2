TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm \
		/home2/users/alumnes/1215560/dades/linux/IDI/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h \
		/home2/users/alumnes/1215560/dades/linux/IDI/Model/model.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp \
        /home2/users/alumnes/1215560/dades/linux/IDI/Model/model.cpp
