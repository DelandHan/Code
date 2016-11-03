#pragma once
#include "WndBoard.h"
#include "DisplayObj.h"
#include "CenterUnit.h"
#include <map>

//manage display, params and connect to the data
class DisplayHub
	:public IUIHub
{
public:
	DisplayHub();
	~DisplayHub();

	int initialize(IDataHub * datasource);

	void refreshItemPanel(ItemPool *itemlist, int panelID) override;

	void displayPath(std::wstring &path) override;

	void getClipboard(std::string & buff) override;

private:
	WndBoard theDisplayBoard;

	ItemPanel theItemPanel[2];
	AttPanel theAttPanel;

	DisplayButton theUpButton;

	//msg child handel
	int activeItemPanel(int id, LPNMHDR data);
	int activeAttPanel(LPNMHDR data);

	int beNotified(WPARAM wp, LPARAM lp);
	int onCommand(WPARAM wp, LPARAM lp);

	CenterUnit theCenterUnit;
};
