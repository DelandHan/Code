#pragma once
#include <list>

//manage the layout
class LayoutManager {
public:
	//add objs
	void addAutoObj(IMoveableWndObj* obj, autownd::vec pos, autownd::vec size);
	void addFixedObj(IMoveableWndObj* obj, autownd::vec pos, autownd::vec size);

	//move objs;
	void moveAutoObj(RECT* rect);
	void moveFixedObj(RECT* rect);
private:
	struct ObjDetail {
		IMoveableWndObj* obj;
		autownd::vec pos, size;
	};
	std::list<ObjDetail> theAutoObjList, theFixedObjList;
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

private:
	//keep the layout
	int updateLayout(WPARAM wp, LPARAM lp);

	//window objects
	autownd::WndObj theMainWnd;

	//message map
	autownd::MsgProcSet theMsgSet;

	LayoutManager theLayoutMgr;

};

