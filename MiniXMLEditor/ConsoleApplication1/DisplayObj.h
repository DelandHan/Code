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
	int initialize(autownd::WndObj *parent) override;
	void drag(int x, int y, int width, int height) override;

	HWND wnd() override { return theObj.wnd(); }

	//clear
	void clear();

	//add att
	void addAttribute(std::wstring &key, std::wstring &value);

private:
	autownd::List theObj;
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