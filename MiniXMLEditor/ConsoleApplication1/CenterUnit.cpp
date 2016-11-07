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
	COPY
};

BaseUnit::BaseUnit()
	:theUI(nullptr), theDataPool(nullptr), thePage{ 0,0 }, theAttPage(0)
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

	wstring path;
	theDataPool->queryPath(page, path);
	theUI->displayPath(path);

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
	theAttPage = item;
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
	getData()->setItemAtt(getAtt(), oldkey, value, nekey);
	showAttribute(getAtt());
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

	menu->emplace_back();
	if (row != -1) menu->emplace_back(LVData(L"Copy", COMMAND::COPY));
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
	case COMMAND::INSERT_A:
	{
		if (checkoutset->size())
		{
			LPARAM node = checkoutset->back().param();
			getData()->addItem(node, "<NewNode />", IDataHub::AFTER, checkoutset);
		}
		else
		{
			getData()->addItem(getPage(panelId), "<NewNode />", IDataHub::CHILD, checkoutset);
		}
	}
	break;
	case COMMAND::INSERT_B:
	{
		ItemPool record;
		LPARAM node = checkoutset->back().param();
		getData()->addItem(node, "<NewNode />", IDataHub::BEFORE, &record);
		checkoutset->insert(checkoutset->begin(), record.begin(), record.end());

	}
	break;
	case COMMAND::CHANGE:
	{
		for (ItemPool::iterator it = checkoutset->begin(); it != checkoutset->end(); it++)
		{
			getData()->queryItem(&*it);
			it->setType(it->type() == 1 ? 3 : 1);
			getData()->setItem(&*it);
		}
	}
	break;
	case COMMAND::COPY:
	{
		string buff = getData()->parse(checkoutset);
		getUI()->saveClipboard(buff);
	}
	break;
	case COMMAND::PASTE:
	{
		string buff;
		getUI()->getClipboard(buff);
		if (buff.size() == 0) return 1;

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

int CenterUnit::fileOperation(std::string command)
{
	if (command.substr(0, 3) == "new") 
	{
		getData()->reset();
		connect(getUI(), getData()); //reset
	}

	return 0;
}

int CenterUnit::inputCmd(std::wstring cmd)
{
	ItemData temp = { cmd,0,0 };
	if (getData()->inputCmd(getPage(0), temp.str()))
	{
		return 1;
	}
	else
	{
		changePage(getPage(0));
		return 0;
	}
}

