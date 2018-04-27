#include <QLCDNumber>

class MyLCDNumber: public QLCDNumber {
	Q_OBJECT
	
	public:
	MyLCDNumber(QWidget *parent);
	
	public slots:
	void clearLCD();
	void displayColor(int);
};
