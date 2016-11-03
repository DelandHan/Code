#include "stdafx.h"
#include "CenterUnit.h"

using namespace std;

enum COMMAND
{
	DEL = 5000,
	CHANGE,
	INSERT_A,
	INSERT_B,
	APPEND,
	PASTE,
};

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
	refreshCurrent();

	return ui == nullptr || data == nullptr ? 1 : 0;
}

int BaseCenterUnit::refreshCurrent()
{
	ItemPool pool;

	if (theData->getItemData(theCurrent[0], &pool) == 0)
	{
		theUI->refreshItemPanel(&pool, 0);
		
		wstring path;
		if (theData->queryPath(theCurrent[0], path) == 0) theUI->displayPath(path);

		refreshChild();

		return 0;
	}
	else
		return 1;

}

int BaseCenterUnit::refreshChild()
{
	ItemPool itempool;
	theData->getItemData(theCurrent[1], &itempool);
	theUI->refreshItemPanel(&itempool, 1);
	return 0;
}

int BaseCenterUnit::refreshAtt()
{

	return 0;
}

int BaseCenterUnit::setCurrent(LPARAM param, int i)
{
	if (i)
	{
		theCurrent[i] = param;
	}
	else
	{
		ItemPool pool;
		pool.push_back({ L"",0,param });
		theData->getItemData(0, &pool);

		ItemData &data = pool.back();
		if (data.getType() != 1 && data.getType() != 9) return 1; //not an element node nor document node

		theCurrent[1] = 0;
		theSelected = 0;
		theCurrent[0] = data.param();
	}

	return 0;
}

int BaseCenterUnit::setSelection(LPARAM param)
{
	theSelected = param;
	return 0;
}

//////////////////////////////////////////////////////

CenterUnit::CenterUnit()
	:theStagePanel(0), theStageParam(0)
{
}

CenterUnit::~CenterUnit()
{
}


int CenterUnit::goHighLevel()
{
	if (setCurrent(datapool()->queryParent(current(0)), 0) == 0) return refreshCurrent();
	return 1;
}

int CenterUnit::getMenu(LVPool * data, int panelID, LPARAM param)
{
	if (current(panelID) == 0) return 1; //blank panel

	theStagePanel = panelID; theStageParam = param;
	if (param)
	{
		data->emplace_back(LVData(L"Del", COMMAND::DEL));
		data->emplace_back(LVData(L"Insert After", COMMAND::INSERT_A));
		data->emplace_back(LVData(L"Insert Before", COMMAND::INSERT_B));
		data->emplace_back(LVData(L"Change", COMMAND::CHANGE));

	}
	else
	{
		data->emplace_back(LVData(L"New", COMMAND::APPEND));
	}

	data->emplace_back(LVData(L"Paste", COMMAND::PASTE));

	return 0;
}

int CenterUnit::setMenuResult(int param)
{
	switch (param)
	{
	case COMMAND::DEL:
	{
		delSelect();
	}
	break;
	case COMMAND::INSERT_A:
	{
		if (theStageParam) 
		{
			if (datapool()->addItem(theStageParam, "<NewNode />",IDataHub::AddAction::AFTER) == 0)
			{
				deselect(theStagePanel);
			}
			else
				return 1;

		}
		else
		{
			if (datapool()->addItem(current(theStagePanel), "<NewNode />", IDataHub::AddAction::AFTER) == 0) deselect(theStagePanel);
		}
	}
	break;
	case COMMAND::INSERT_B:
	{
		if (datapool()->addItem(theStageParam, "<NewNode />", IDataHub::AddAction::BEFORE) == 0)
		{
			deselect(theStagePanel);
		}
		else
			return 1;
	}
	break;
	case COMMAND::CHANGE:
	{
		ItemPool pool;
		pool.push_back({ L"",0,param });
		datapool()->getItemData(0, &pool);

		ItemData &data = pool.back();


		data.setType(data.getType() == 1 ? 3 : 1);
		datapool()->setItem(&data);
		deselect(theStagePanel);
	}
	break;
	case COMMAND::PASTE:
	{
		string buff;
		uimodule()->getClipboard(buff);
		if(theStageParam) datapool()->addItem(theStageParam, buff,IDataHub::AddAction::AFTER);
		else datapool()->addItem(current(theStagePanel), buff, IDataHub::AddAction::APPEND);
		deselect(theStagePanel);
	}
	break;
	case COMMAND::APPEND:
	{
		if (datapool()->addItem(current(theStagePanel), "<NewNode />", IDataHub::AddAction::APPEND) == 0) deselect(theStagePanel);
	}
	break;
	default:
		return 1;
	}

	theStagePanel = 0;
	theStageParam = 0;
	return 0;
}

int CenterUnit::delSelect()
{
	ItemData data = { L"",0,selection() };

	LPARAM parent = datapool()->queryParent(selection());
	if (datapool()->setItem(&data) == 0)
	{
		setSelection(0);//de-select
		if (parent == current(0)) {
			if (setCurrent(0, 1) == 0) 	refreshCurrent();
		}
		else
			refreshChild();

		return 0;
	}
	else
		return 1;
}

void CenterUnit::deselect(int panel)
{
	setSelection(0);

	if (panel == 0) {
		setCurrent(0, 1);
		refreshCurrent();
	}
	refreshChild();
	refreshAtt();
}

int CenterUnit::select(LPARAM param, int panelId)
{
	if (panelId == 0) {
		if (setCurrent(param, 1) == 0) refreshChild();
	}
	if (setSelection(param) == 0) refreshAtt();

	return 0;
}

int CenterUnit::dbClick(LPARAM param)
{
	if (setCurrent(param, 0) == 0) return refreshCurrent();
	else return 1;
}

int CenterUnit::edit(LPARAM param, std::wstring &str)
{
	ItemPool pool;
	pool.push_back({ str, 0,param });

	ItemData &data = pool.back();

	if (str == L"") return 1;

	if (datapool()->setItem(&data) == 0)
	{
		datapool()->getItemData(0, &pool);
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

