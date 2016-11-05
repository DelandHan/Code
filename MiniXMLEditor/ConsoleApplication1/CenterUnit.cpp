#include "stdafx.h"
#include "CenterUnit.h"

using namespace std;

enum COMMAND
{
	DEL = 5000,
	CHANGE,
	INSERT_A,
	INSERT_B,
	PASTE,
};

BaseUnit::BaseUnit()
	:theUI(nullptr), theDataPool(nullptr), thePage{ 0,0 }
{
}

BaseUnit::~BaseUnit()
{
}

int BaseUnit::changePage(LPARAM page)
{
	ItemPool pool;
	if (page) theDataPool->getChildItemData(page, &pool);

	thePage[0] = page;
	theUI->refreshItemPanel(&pool, 0);

	showDetail(0);

	return 0;
}

int BaseUnit::showDetail(LPARAM item)
{
	ItemPool pool;
	if (item) theDataPool->getChildItemData(item, &pool);

	thePage[1] = item;
	theUI->refreshItemPanel(&pool, 1);

	showAttribute(item);

	return 0;
}

int BaseUnit::showAttribute(LPARAM item)
{
	AttPool pool;
	if (item) theDataPool->getItemAtt(item, &pool);
	theUI->refreshAttPanel(&pool);

	return 0;
}

int BaseUnit::connect(IUIHub * ui, IDataHub * data)
{
	theUI = ui;
	theDataPool = data;

	ItemData root = { L"",0,0 };
	data->queryItem(&root);
	changePage(root.param());

	return 0;
}

//////////////////////////////////////////////


CenterUnit::CenterUnit()
{
}

CenterUnit::~CenterUnit()
{
}



int CenterUnit::goHighLevel()
{
	return changePage(getData()->queryParent(getPage(0)));
}

int CenterUnit::select(LPARAM item, int panelId)
{
	if (item == 0) return 1;
	if (panelId) showAttribute(item);
	else showDetail(item);
	return 0;
}

int CenterUnit::dbClick(LPARAM param)
{
	ItemData item = { L"",0,param };
	getData()->queryItem(&item);
	if (item.type() != 1) return 1;
	return changePage(param);
}

int CenterUnit::edit(ItemPool *checkoutset)
{
	for (std::list<ItemData>::iterator it = checkoutset->begin(); it != checkoutset->end(); it++)
	{
		if (getData()->setItem(&*it) == 0)
		{
			getData()->queryItem(&*it);
			//to do. check whether there is an error here.
			return 0;
		}
	}
	return 1;
}

int CenterUnit::updateAtt(const TCHAR * oldkey, const TCHAR * value, const TCHAR * nekey)
{
	return 0;
}

int CenterUnit::getMenu(LVPool * menu, int panelID, int row)
{
	if (row == -1)
	{
		if (getPage(panelID) == 0) return 1;
	}

	if (row != -1)
	{
		menu->emplace_back(LVData(L"Del", COMMAND::DEL));
		menu->emplace_back(LVData(L"Insert After", COMMAND::INSERT_A));
		menu->emplace_back(LVData(L"Insert Before", COMMAND::INSERT_B));
		menu->emplace_back(LVData(L"Change", COMMAND::CHANGE));

	}
	else
	{
		menu->emplace_back(LVData(L"New", COMMAND::INSERT_A));
	}

	menu->emplace_back(LVData(L"Paste", COMMAND::PASTE));

	return 0;
}

int CenterUnit::setMenuResult(int command, int panelId, ItemPool * checkoutset)
{
	switch (command)
	{
	case COMMAND::DEL:
	{
		for (std::list<ItemData>::iterator it = checkoutset->begin(); it != checkoutset->end();)
		{
			it->setStr(L"");
			if (getData()->setItem(&*it) == 0) it = checkoutset->erase(it);
			else it++;
		}
	}
	break;
	/*case COMMAND::INSERT_A:
	{
		if (theStageParam)
		{
			if (datapool()->insertAfter(theStageParam, "<NewNode />") == 0)
			{
				deSelect(theStagePanel);
			}
			else
				return 1;

		}
		else
		{
			if (datapool()->append(current(theStagePanel), "<NewNode />") == 0) deSelect(theStagePanel);
		}
	}
	break;
	case COMMAND::INSERT_B:
	{
		if (datapool()->insertBefore(theStageParam) == 0)
		{
			deSelect(theStagePanel);
		}
		else
			return 1;
	}
	break;
	case COMMAND::CHANGE:
	{
		ItemData data = { L"",0,theStageParam };
		datapool()->queryItem(&data);
		data.setType(data.type() == 1 ? 3 : 1);
		datapool()->setItem(&data);
		deSelect(theStagePanel);
	}
	break;*/
	case COMMAND::PASTE:
	{
		string buff;
		getUI()->getClipboard(buff);

		if (checkoutset->size())
		{
			LPARAM node = checkoutset->back().param();
			getData()->addItem(node, buff, IDataHub::AFTER, checkoutset);
		}
		else
		{
			getData()->addItem(getPage(panelId), buff, IDataHub::CHILD, checkoutset);
		}
	}
	break;
	default:
		return 1;
	}
	return 0;
}


/*




int CenterUnit::setMenuResult(int command)
{
	switch (command)
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
			if (datapool()->insertAfter(theStageParam, "<NewNode />") == 0)
			{
				deSelect(theStagePanel);
			}
			else
				return 1;

		}
		else
		{
			if (datapool()->append(current(theStagePanel), "<NewNode />") == 0) deSelect(theStagePanel);
		}
	}
	break;
	case COMMAND::INSERT_B:
	{
		if (datapool()->insertBefore(theStageParam) == 0)
		{
			deSelect(theStagePanel);
		}
		else
			return 1;
	}
	break;
	case COMMAND::CHANGE:
	{
		ItemData data = { L"",0,theStageParam };
		datapool()->queryItem(&data);
		data.setType(data.type() == 1 ? 3 : 1);
		datapool()->setItem(&data);
		deSelect(theStagePanel);
	}
	break;
	case COMMAND::PASTE:
	{
		string buff;
		uimodule()->getClipboard(buff);
		if(theStageParam) datapool()->insertAfter(theStageParam, move(buff));
		else datapool()->append(current(theStagePanel), move(buff));
		deSelect(theStagePanel);
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

void CenterUnit::deSelect(int panel)
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


int CenterUnit::updateAtt(const TCHAR * oldkey, const TCHAR * value, const TCHAR * nekey)
{
	datapool()->setItemAtt(selection(), oldkey, value, nekey);
	
	refreshAtt();
	return 0;
}

*/
