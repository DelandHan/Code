#pragma once
#include <string>
#include <list>
#include "ItemData.h"

//the Obj used for UI
class IWndObj 
{
public:
	virtual int initialize(autownd::WndObj *parent) = 0;
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
	virtual void refreshItemPanel(ItemPool *itemlist, int panelID) = 0;
	virtual void refreshAttPanel(AttPool * attlist) = 0;
	virtual void displayPath(std::wstring &path) = 0;

	virtual void getClipboard(std::string & buff) = 0;
};

class IInputHub
{
public:
	virtual int select(LPARAM param, int panelId) = 0;
	virtual int dbClick(LPARAM param) = 0;
	virtual int edit(LPARAM param, std::wstring &str) = 0;
	virtual int goHighLevel() = 0;

	virtual int getMenu(LVPool * data, int panelID, LPARAM param) = 0;
	virtual int setMenuResult(int param) = 0;

	virtual int updateAtt(const TCHAR * oldkey, const TCHAR * value, const TCHAR * nekey) = 0;
};

//DataHub
class IDataHub
{
public:
	virtual int getChildItemData(LPARAM param, ItemPool *pool) = 0;
	virtual int getItemAtt(LPARAM param, AttPool *pool) = 0;
	virtual LPARAM queryParent(LPARAM param) = 0;
	virtual int queryItem(ItemData *pool) = 0;
	virtual int queryPath(LPARAM param, std::wstring &path) = 0;

	virtual int setItem(ItemData *source) = 0;
	virtual int setItemAtt(LPARAM param, std::wstring oldkey, std::wstring value, std::wstring nekey) = 0;

	virtual int insertAfter(LPARAM param, std::string text) = 0;
	virtual int append(LPARAM parent, std::string text) = 0; 

	virtual int insertBefore(LPARAM param) = 0;


/*	virtual int delItem(LPARAM param) = 0;
*/
};