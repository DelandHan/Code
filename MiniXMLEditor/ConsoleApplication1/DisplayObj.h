#pragma once

class ItemPanel :
	public IMoveableWndObj
{
public:
	int initialize(autownd::WndObj *parent) override;
	void drag(int x, int y, int width, int height) override;

	HWND wnd() override { return theObj.wnd(); }

	//add items
	void addItems(std::wstring &str, int image, LPARAM param);
	//clear
	void clear();
	//set Text
	void setItemText(int item, int subitem, TCHAR* str);

	//get param
	LPARAM getParam(int item);

private:
	autownd::List theObj;
};

class AttPanel : 
	public IMoveableWndObj
{
public:
	AttPanel();
	int initialize(autownd::WndObj *parent) override;
	void drag(int x, int y, int width, int height) override;

	HWND wnd() override { return theObj.wnd(); }

	//clear
	void clear();

	//startediting
	void startEditing(int item, int subitem, autownd::IMsgProcess *proc);

	//add att
	void addAttribute(std::wstring &key, std::wstring &value);

	//get Editing result
	std::wstring *getEditResult();

	inline int count() { return theObj.getCount(); }

private:
	//call back for editing
	int finishEditing(WPARAM wp, LPARAM lp);

	autownd::List theObj;

	autownd::Edit theEdit;
	std::wstring theBuff[3];
	int theOnEditing;
	autownd::MsgProc<AttPanel> theCallBack;

	autownd::IMsgProcess *theParentCallBack;
};

class DisplayButton :
	public IMoveableWndObj
{
public:
	int initialize(autownd::WndObj *parent) override;
	void drag(int x, int y, int width, int height) override;

	HWND wnd() override { return theObj.wnd(); }

private:
	autownd::WndObj theObj;
};