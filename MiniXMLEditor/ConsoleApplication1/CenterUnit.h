#pragma once

class CenterUnit
{
public:
	CenterUnit();
	~CenterUnit();

	int connect(IUIHub * ui, IDataHub* data);

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

	int getMenu(LVPool * menu, LPARAM param);
	int setMenuResult(int command);


private:
	int changePage(LPARAM page);
	int showDetail(LPARAM item);
	int showAttribute(LPARAM item);

	IUIHub * theUI;
	IDataHub* theDataPool;

	LPARAM theCurrentPage;
};
