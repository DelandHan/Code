#include "stdafx.h"
#include "CenterUnit.h"

using namespace std;

BaseCenterUnit::BaseCenterUnit()
	:theSelected(0), theUI(nullptr)
{
	theCurrent[0] = 0;
	theCurrent[1] = 0;
}

BaseCenterUnit::~BaseCenterUnit()
{
}

int BaseCenterUnit::connect(IUIHub * ui, IDataHub* data)
{
	theUI = ui;
	theData = data;
	
	setCurrent(0, 0);
	setCurrent(0, 1);
	setSelection(0);

	return ui == nullptr || data == nullptr ? 1 : 0;
}

int BaseCenterUnit::refreshCurrent()
{
	ItemPool pool;

	if (theData->getChildItemData(theCurrent[0], &pool) == 0)
	{
		theUI->refreshItemPanel(&pool, 0);
		
		wstring path;
		if (theData->queryPath(theCurrent[0], path) == 0) theUI->displayPath(path);

		refreshChild();
		refreshAtt();
		return 0;
	}
	else
		return 1;

}

int BaseCenterUnit::refreshChild()
{
	ItemPool itempool;
	theData->getChildItemData(theCurrent[1], &itempool);
	theUI->refreshItemPanel(&itempool, 1);
	return 0;
}

int BaseCenterUnit::refreshAtt()
{
	AttPool attpool;
	theData->getItemAtt(theSelected, &attpool);
	theUI->refreshAttPanel(&attpool);

	return 0;
}

int BaseCenterUnit::setCurrent(LPARAM param, int i)
{
	ItemData data = { L"",0,param };
	theData->queryItem(&data);

	if (data.type() != 1 && data.type() != 9) return 1; //not an element node nor document node

	theCurrent[i] = data.param();

	if (i == 0)
	{
		theCurrent[1] = 0;
		theSelected = 0;
		return refreshCurrent();
	}
	else
	{
		return refreshChild();
	}
}

int BaseCenterUnit::setSelection(LPARAM param)
{
	theSelected = param;
	return refreshAtt();
}

//////////////////////////////////////////////////////

CenterUnit::CenterUnit()
{
}

CenterUnit::~CenterUnit()
{
}


int CenterUnit::goHighLevel()
{
	return setCurrent(datapool()->queryParent(current(0)),0);
}

int CenterUnit::select(LPARAM param, int panelId)
{
	if (panelId == 0) setCurrent(param, 1);
	setSelection(param);

	return 0;
}

int CenterUnit::dbClick(LPARAM param)
{
	return setCurrent(param, 0);
}

int CenterUnit::edit(LPARAM param, std::wstring &str)
{
	ItemData data = { str,0,param };

	if (str == L"") return 1;

	if (datapool()->setItem(&data) == 0)
	{
		datapool()->queryItem(&data);
		str = data.strW(); //it might change
		return 0;
	}
	return 1;
}

int CenterUnit::updateAtt(const TCHAR * oldkey, const TCHAR * value, const TCHAR * nekey)
{
	datapool()->setItemAtt(selection(), oldkey, value, nekey);
	
	refreshAtt();
	return 0;
}

