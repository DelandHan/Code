#pragma once

class CenterUnit
{
public:
	CenterUnit();
	~CenterUnit();

	int connect(IUIHub * ui, IDataHub* data);

	int changeCurrent(LPARAM param);
	int changeSelectOnCurrent(LPARAM param);
	int changeSelectOnChild(LPARAM param);
	int goHighLevel();

private:
	IUIHub * theUI;
	IDataHub* theData;

	LPARAM theCurrent, theSelected;
};

class InputUnit :
	public IInputHub
{
public:
	InputUnit();
	~InputUnit();

	int initialize();

	int connectTo(CenterUnit* unit);

	int select(LPARAM param, int panelId)override;
	int dbClick(LPARAM param)override;
	int edit(LPARAM param, TCHAR * str)override;
	int goHighLevel()override;

private:
	CenterUnit *theCenter;
};

