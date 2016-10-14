#pragma once

class ItemPanel 
	:public IMoveableWndObj
{
public:
	int initialize(autownd::WndObj *parent);
	void drag(int x, int y, int width, int height) override;

	//add items
	void addItems(std::wstring &str, int image, LPARAM param);

private:
	autownd::List theObj;
};

class AttPanel 
	:public IMoveableWndObj 
{
public:
	int initialize(autownd::WndObj *parent);
	void drag(int x, int y, int width, int height) override;

	//add att
	void addAttribute(std::wstring &key, std::wstring &value);

private:
	autownd::List theObj;
};

class DisplayButton 
	:public IMoveableWndObj 
{
public:
	int initialize(autownd::WndObj *parent);
	void drag(int x, int y, int width, int height) override;

private:
	autownd::WndObj theObj;
};