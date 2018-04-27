#include "MyLCDNumber.h"

MyLCDNumber::MyLCDNumber(QWidget *parent=0):QLCDNumber(parent) {}

void MyLCDNumber::clearLCD() {
	display(0);
}

/*void MyLCDNumber::displayColor(int i) {
	if (i == 0) {
		
	}
	else if (i%2 == 0) {

	}
	else {

	}
	display(i);
}*/