#pragma once

class BaseUnit
{
public:
	BaseUnit();
	~BaseUnit();

	int connect(IUIHub * ui, IDataHub* data);
	
protected:
	int changePage(LPARAM page);
	int showDetail(LPARAM item);
	int showAttribute(LPARAM item);

	inline IUIHub * getUI() { return theUI; }
	inline IDataHub* getData() { return theDataPool; }
	inline LPARAM getPage(size_t i) { return thePage[i]; }
	inline LPARAM getAtt() { return theAttPage; }

private:
	IUIHub * theUI;
	IDataHub* theDataPool;

	LPARAM thePage[2], theAttPage;

};

class CenterUnit
	:public BaseUnit
{
public:
	CenterUnit();
	~CenterUnit();

	////////browse///////

	//up
	int goHighLevel();
	//select
	int select(LPARAM item, int panelId);
	//dbclick
	int dbClick(LPARAM param);

	/////////modify///////
	int edit(ItemPool *checkoutset);
	int updateAtt(const TCHAR * oldkey, const TCHAR * value, const TCHAR * nekey);

	int getMenu(LVPool * menu, int panelID, int row);
	int setMenuResult(int command, int panelId, ItemPool * checkoutset);

	int fileOperation(std::string command);


private:

};
