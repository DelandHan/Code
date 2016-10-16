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

int CenterUnit::changeCurrent(LPARAM param)
{
	ItemPool pool;

	if (theData->getChildItemData(param, &pool) == 0)
	{
		theUI->refreshItemPanel(&pool);
		theUI->refreshChildPanel(0);
		theUI->refreshAttPanel(0);
		theCurrent = param;
		return 0;
	}
	else
		return 1;
}

int CenterUnit::changeSelectOnCurrent(LPARAM param)
{
	ItemPool itempool;

	if(theData->getChildItemData(param, &itempool)==0) 
		theUI->refreshChildPanel(&itempool);

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
	return changeCurrent(theData->queryParent(theCurrent));
}


InputUnit::InputUnit()
{
}


InputUnit::~InputUnit()
{
}

int InputUnit::initialize()
{
	return 0;
}

int InputUnit::connectTo(CenterUnit * unit)
{
	theCenter = unit;
	return unit == nullptr ? 1 : 0;
}

int InputUnit::select(LPARAM param, int panelId)
{
	if (panelId) return theCenter->changeSelectOnChild(param);
	else return theCenter->changeSelectOnCurrent(param);
}

int InputUnit::dbClick(LPARAM param)
{
	return theCenter->changeCurrent(param);
}

int InputUnit::edit(LPARAM param, TCHAR * str)
{
	return 1;
}

int InputUnit::goHighLevel()
{
	return theCenter->goHighLevel();
}
