#pragma once

class CenterUnit
	:public IInputHub
{
public:
	CenterUnit();
	~CenterUnit();

	int connect(IUIHub * ui, IDataHub* data);

	int select(LPARAM param, int panelId)override;
	int dbClick(LPARAM param)override;
	int edit(LPARAM param, std::wstring &str)override;
	int goHighLevel()override;

	int updateAtt(std::wstring *oldkey, std::wstring *value, std::wstring *nekey)override;


private:
	int changeSelectOnCurrent(LPARAM param);
	int changeSelectOnChild(LPARAM param);

	IUIHub * theUI;
	IDataHub* theData;

	LPARAM theCurrent, theSelected;
};
