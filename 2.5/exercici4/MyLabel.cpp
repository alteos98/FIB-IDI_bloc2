#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent=0):QLabel(parent) {}

void MyLabel::setTextMajuscules(const QString &s) {
	QString str = s.toUpper();
	setText(str);
}

void MyLabel::truncaText(int i) {
	QString str = text();
	str.truncate(i);
	setText(str);
}