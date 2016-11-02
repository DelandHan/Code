#pragma once

class BaseCenterUnit
{
public:
	BaseCenterUnit();
	~BaseCenterUnit();

	int connect(IUIHub * ui, IDataHub* data);

protected:
	int refreshCurrent();
	int refreshChild();
	int refreshAtt();

	int setCurrent(LPARAM param, int i);
	int setSelection(LPARAM param);

	inline IUIHub * uimodule() { return theUI; }
	inline IDataHub* datapool() { return theData; }
	inline LPARAM current(int i) { return theCurrent[i]; }
	inline LPARAM selection() { return theSelected; }

private:
	IUIHub * theUI;
	IDataHub* theData;

	LPARAM theCurrent[2], theSelected;
};

class CenterUnit
	:public BaseCenterUnit
{
public:
	CenterUnit();
	~CenterUnit();


	int select(LPARAM param, int panelId);
	int dbClick(LPARAM param);
	int edit(LPARAM param, std::wstring &str);
	int goHighLevel();

	int getMenu(LVPool * data, int panelID, LPARAM param) ;
	int setMenuResult(int param) ;

	int delSelect();
	void deselect(int panel);

	int updateAtt(const TCHAR * oldkey, const TCHAR * value, const TCHAR * nekey);


private:
	int theStagePanel;
	LPARAM theStageParam;

};
