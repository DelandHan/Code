#pragma once
#include "WndBoard.h"
#include "DisplayObj.h"
#include <map>

//manage display, params and connect to the data
class DisplayHub
{
public:
	DisplayHub();
	~DisplayHub();

	int initialize();

	int setDataSeed(ILine* seed);


private:
	WndBoard theDisplayBoard;

	ItemPanel theItemPanel[2];
	ILine::Page theDataPage[2];
	AttPanel theAttPanel;

	DisplayButton theUpButton;

	//page build function
	int buildPanelPage(ILine* line, int id);

	//msg child handel
	int activeItemPanel(int id, LPNMHDR data);
	int activeAttPanel(LPNMHDR data);

	int beNotified(WPARAM wp, LPARAM lp);
	int onCommand(WPARAM wp, LPARAM lp);
};
