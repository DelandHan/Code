#pragma once
#include "DisplayObj.h"
#include "WndBoard.h"

class TreePanel
{
public:
	TreePanel();
	~TreePanel();

	int initialize();

	void setXMLNode(xml::XMLNode * node);

private:
	ItemPanel theChildPanel, theDetailPanel;
	AttPanel theAttPanel;

	WndBoard theBoard;

	void fill(ItemPanel * panel, xml::XMLNode * node);

	int beNotified(WPARAM wp, LPARAM lp);
	int onCommand(WPARAM wp, LPARAM lp);

};

