#pragma once
#include "WndDisplay.h"

//manage display, params and connect to the data
class DisplayHub
{
public:
	DisplayHub();
	~DisplayHub();

	int initialize();

	int beNotified(WPARAM wp, LPARAM lp);
	int onCommand(WPARAM wp, LPARAM lp);

private:
	WndDisplay theDisplayBoard;
};

