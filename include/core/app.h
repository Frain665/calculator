#ifndef APP_H
#define APP_H

#include "wx/wx.h"

class CalculatorApp : public wxApp
{
	bool OnInit() override;
	int OnExit() override;
};

#endif //APP_H