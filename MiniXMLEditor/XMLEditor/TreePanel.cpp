#include "stdafx.h"
#include "TreePanel.h"
#include <string>

using namespace std;
using namespace xml;

void convertToWStr(std::wstring & dest, const std::string & source)
{
	dest.resize(source.size());
	std::copy(source.begin(), source.end(), dest.begin());
}

void convertToStr(std::string & dest, const std::wstring & source)
{
	dest.resize(source.size());
	WideCharToMultiByte(CP_ACP, 0, &source[0], (int)source.size(), &dest[0], dest.size(), 0, 0);
}

TreePanel::TreePanel()
{
}


TreePanel::~TreePanel()
{
}

int TreePanel::initialize()
{
	theBoard.initialize();

	if (theBoard.connectDisplayObj(&theChildPanel, { 0,0 }, { 50,100 }, 1)) return 1;
	if (theBoard.connectDisplayObj(&theDetailPanel, { 50,0 }, { 50,50 }, 1)) return 1;
	if (theBoard.connectDisplayObj(&theAttPanel, { 50,50 }, { 50,50 }, 1)) return 1;

	theBoard.getMsgSet()->addMsgProc(WM_NOTIFY, this, &TreePanel::beNotified);
	theBoard.getMsgSet()->addMsgProc(WM_COMMAND, this, &TreePanel::onCommand);

	theBoard.update();
	return 0;
}

void TreePanel::setXMLNode(xml::XMLNode * node)
{
	fill(&theChildPanel, node);
}

void TreePanel::fill(ItemPanel * panel, xml::XMLNode * node)
{
	wstring buff;

	XMLNode * child = node->getFirstChild();

	while (child) 
	{
		convertToWStr(buff, child->getString());
		panel->addItems(buff, node->getType(), (LPARAM)node);
		
		child = child->getNext();
	}
}

int TreePanel::beNotified(WPARAM wp, LPARAM lp)
{
	LPNMHDR data = (LPNMHDR)lp;
	LPNMITEMACTIVATE param = (LPNMITEMACTIVATE)data;

	switch (data->code)
	{
	case LVN_ITEMCHANGED:
	{
		XMLNode * node = (XMLNode*)param->lParam;
		fill(&theDetailPanel, node);

	}
	break;
	case NM_DBLCLK:
	{

	}
	break;
	case LVN_ENDLABELEDIT:
	{
		NMLVDISPINFO* info = (NMLVDISPINFO*)data;
		if (info->item.pszText == nullptr) return 1;
		wstring buff = info->item.pszText;
		string buffA;
		convertToStr(buffA, buff);
		XMLNode * node = (XMLNode*)info->item.lParam;
		node->setString(buffA);

	}
	break;
	case NM_RCLICK:
	{

	}
	break;

	default:
		return 1;
	}
	return 0;
}

int TreePanel::onCommand(WPARAM wp, LPARAM lp)
{
	return 0;
}
