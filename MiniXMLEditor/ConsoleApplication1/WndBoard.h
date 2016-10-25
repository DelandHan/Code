#pragma once
#include <list>

//manage the layout
class LayoutManager {
public:
	LayoutManager(int type);
	~LayoutManager();

	//add objs
	void addObj(IMoveableWndObj* obj, autownd::vec pos, autownd::vec size);

	void move(RECT* rect);

private:
	//move objs;
	void moveAutoObj(RECT* rect);
	void moveFixedObj(RECT* rect);

	int theType;

	struct ObjDetail {
		IMoveableWndObj* obj;
		autownd::vec pos, size;
	};
	std::list<ObjDetail> theList;
};

//make a board for holding panels and autosize them
class WndBoard
{
public:
	WndBoard();
	~WndBoard();

	//initialize
	int initialize();

	//to insert msg funs
	autownd::MsgProcSet * getMsgSet() { return &theMsgSet; }

	//build obj
	int connectDisplayObj(IMoveableWndObj* obj, autownd::vec pos, autownd::vec size, int area);

	//update
	void update();

	inline HWND	wnd() { return theMainWnd.wnd(); }

private:
	//keep the layout
	int updateLayout(WPARAM wp, LPARAM lp);

	//window objects
	autownd::WndObj theMainWnd;

	//message map
	autownd::MsgProcSet theMsgSet;

	LayoutManager theLayoutMgr[2];

};

