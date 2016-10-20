#pragma once
#include "WndBoard.h"
#include "DisplayObj.h"
#include <map>

//manage display, params and connect to the data
class DisplayHub
	:public IUIHub
{
public:
	DisplayHub();
	~DisplayHub();

	int initialize();

	int connectToInputHub(IInputHub *inputhub);

	virtual void refreshItemPanel(ItemPool *itemlist, int panelID) override;

	virtual void refreshAttPanel(AttPool * attlist) override;

	void displayPath(std::wstring &path) override;

private:
	WndBoard theDisplayBoard;

	ItemPanel theItemPanel[2];
	AttPanel theAttPanel;

	DisplayButton theUpButton;
	autownd::ContextMenu theMenu;

	IInputHub *theInputHub;

	//msg child handel
	int activeItemPanel(int id, LPNMHDR data);
	int activeAttPanel(LPNMHDR data);

	int beNotified(WPARAM wp, LPARAM lp);
	int onCommand(WPARAM wp, LPARAM lp);
};
