#include "stdafx.h"
#include "CenterUnit.h"

using namespace std;

CenterUnit::CenterUnit()
	:theCurrent(0), theSelected(0), theUI(nullptr)
{
}

CenterUnit::~CenterUnit()
{
}

int CenterUnit::connect(IUIHub * ui, IDataHub* data)
{
	theUI = ui;
	theData = data;
	return ui == nullptr || data == nullptr ? 1 : 0;
}

int CenterUnit::changeSelectOnCurrent(LPARAM param)
{
	ItemPool itempool;

	if (theData->getChildItemData(param, &itempool) == 0)
		theUI->refreshItemPanel(&itempool, 1);

	changeSelectOnChild(param);
	return 0;
}

int CenterUnit::changeSelectOnChild(LPARAM param)
{
	AttPool attpool;
	if (theData->getItemAtt(param, &attpool) == 0) theUI->refreshAttPanel(&attpool);
	else return 1;
	theSelected = param;

	return 0;
}

int CenterUnit::goHighLevel()
{
	return dbClick(theData->queryParent(theCurrent));
}

int CenterUnit::select(LPARAM param, int panelId)
{
	if (panelId) return changeSelectOnChild(param);
	else return changeSelectOnCurrent(param);
}

int CenterUnit::dbClick(LPARAM param)
{
	ItemPool pool;

	if (theData->getChildItemData(param, &pool) == 0)
	{
		theUI->refreshItemPanel(&pool, 0);
		theUI->refreshItemPanel(nullptr, 1);
		theUI->refreshAttPanel(nullptr);
		theCurrent = param;
		return 0;
	}
	else
		return 1;
}

int CenterUnit::edit(LPARAM param, std::wstring &str)
{
	ItemData data = { str,0,param };

	if (theData->setItem(&data) == 0)
		if (theData->queryItem(&data) == 0)
		{
			str = data.str;
			return 0;
		}			

	return 1;
}

int CenterUnit::updateAtt(std::wstring * oldkey, std::wstring * value, std::wstring * nekey)
{
	if (*oldkey == L"")
		theData->setItemAtt(theSelected, nekey, value, nekey);
	else
		theData->setItemAtt(theSelected, oldkey, value, nekey);
	
	changeSelectOnChild(theSelected);
	return 0;
}
