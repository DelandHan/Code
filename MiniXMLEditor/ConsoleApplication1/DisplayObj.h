#pragma once

class ItemPanel
	:public IMoveableWndObj
	,public autownd::List
{
public:
	int initialize(autownd::WndObj *parent) override;
	void drag(int x, int y, int width, int height) override;

	//add items
	void addItems(std::wstring &str, int image, LPARAM param);
	//set Text
	void setItemText(int item, int subitem, TCHAR* str);

	//get param
	LPARAM getParam(int item);

};

class AttPanel
	:public IMoveableWndObj
	,public autownd::List
{
public:
	AttPanel();
	int initialize(autownd::WndObj *parent) override;
	void drag(int x, int y, int width, int height) override;

	//startediting
	void startEditing(int item, int subitem, autownd::IMsgProcess *proc);

	//add att
	void addAttribute(std::wstring &key, std::wstring &value);

	//get Editing result
	const std::wstring *getEditResult();

private:
	//call back for editing
	int finishEditing(WPARAM wp, LPARAM lp);

	autownd::Edit theEdit;
	std::wstring theBuff[3];
	int theOnEditing;
	autownd::MsgProc<AttPanel> theCallBack;

	autownd::IMsgProcess *theParentCallBack;
};

class DisplayButton
	:public IMoveableWndObj
	,public autownd::WndObj
{
public:
	int initialize(autownd::WndObj *parent) override;
	void drag(int x, int y, int width, int height) override;
};