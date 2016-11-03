#pragma once
#include <string>
#include <list>
#include "ItemData.h"

//the obj can be moved. used by UI
class IMoveableWndObj
{
public:
	virtual int initialize(autownd::WndObj *parent) = 0;
	//drag
	virtual void drag(int x, int y, int width, int height) = 0;
};

//ControlCenter
class IUIHub
{
public:
	virtual void refreshItemPanel(ItemPool *itemlist, int panelID) = 0;

	virtual void displayPath(std::wstring &path) = 0;

	virtual void getClipboard(std::string & buff) = 0;
};

//DataHub
class IDataHub
{
public:
	virtual int getItemData(LPARAM parent, ItemPool *pool) = 0;
	
	virtual LPARAM queryParent(LPARAM param) = 0;

	virtual int queryPath(LPARAM param, std::wstring &path) = 0;

	virtual int setItem(ItemData *source) = 0;
	enum AddAction
	{
		APPEND,
		BEFORE,
		AFTER
	};
	virtual int addItem(LPARAM position, std::string text, AddAction action) = 0;

	virtual int setItemAtt(LPARAM param, std::wstring oldkey, std::wstring value, std::wstring nekey) = 0;

};