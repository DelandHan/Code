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
	:public IInputHub
	,public BaseCenterUnit
{
public:
	CenterUnit();
	~CenterUnit();


	int select(LPARAM param, int panelId)override;
	int dbClick(LPARAM param)override;
	int edit(LPARAM param, std::wstring &str)override;
	int goHighLevel()override;

	int getMenu(LVPool * data, int panelID, LPARAM param) override;
	int setMenuResult(int param) override;

	int delSelect();
	void deSelect(int panel);

	int updateAtt(const TCHAR * oldkey, const TCHAR * value, const TCHAR * nekey)override;


private:

	int theStagePanel;
	LPARAM theStageParam;

};
