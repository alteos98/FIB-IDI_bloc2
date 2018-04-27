TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm \
	/home/alteos98/Documents/UPC/IDI/model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h \
	/home/alteos98/Documents/UPC/IDI/model/model.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp \
        /home/alteos98/Documents/UPC/IDI/model/model.cpp
