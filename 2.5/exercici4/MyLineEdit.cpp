#include "MyLineEdit.h"

MyLineEdit::MyLineEdit(QWidget *parent=0):QLineEdit(parent) {}

void MyLineEdit::tractaReturn() {
	emit enviaText(text());
}
