#pragma once
#include <string>
#include <list>

//passing data
typedef std::pair<std::pair<std::wstring, int>, LPARAM> ItemData;
typedef std::list<ItemData> ItemPool;

typedef std::pair<std::wstring, std::wstring> AttData;
typedef std::list<AttData> AttPool;

//the Obj used for UI
class IWndObj 
{
public:
	virtual int initialize(autownd::WndObj *parent) = 0;
	virtual HWND wnd() = 0;
};

//the obj can be moved. used by UI
class IMoveableWndObj :public IWndObj
{
public:
	//drag
	virtual void drag(int x, int y, int width, int height) = 0;
};

//ControlCenter
class IUIHub
{
public:
	virtual void refreshItemPanel(ItemPool *itemlist) = 0;
	virtual void refreshChildPanel(ItemPool *itemlist) = 0;

	virtual void refreshAttPanel(AttPool * attlist) = 0;
};

class IInputHub
{
public:
	virtual int select(LPARAM param, int panelId) = 0;
	virtual int dbClick(LPARAM param) = 0;
	virtual int edit(LPARAM param, TCHAR * str) = 0;
	virtual int goHighLevel() = 0;
};

//DataHub
class IDataHub
{
public:
	virtual int getChildItemData(LPARAM param, ItemPool *pool) = 0;
	virtual int getItemAtt(LPARAM param, AttPool *pool) = 0;

	virtual LPARAM queryParent(LPARAM param) = 0;
};