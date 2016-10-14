#pragma once
#include <list>
#include "DisplayObj.h"

class WndMsgSet 
{
public:
	WndMsgSet() {}
	~WndMsgSet();

	void addMsgProc(UINT msg, autownd::IMsgProcess *msgproc);
	template<class T> void addMsgProc(UINT msg, T* obj, int(T::*fun)(WPARAM wp, LPARAM lp)) {
		autownd::MsgProc<T> * temp = new autownd::MsgProc<T>(obj, fun);
		addMsgProc(msg, temp);
		theList.push_back(temp);
	}
	inline autownd::MsgSet * map() { return &theMsgMap; }
private:
	autownd::MsgSet theMsgMap;
	std::list<autownd::IMsgProcess*> theList;
};

//make a board for holding panels and autosize them
class WndDisplay
{
public:
	WndDisplay();
	~WndDisplay();

	//initialize
	int initialize();

	//to insert msg funs
	WndMsgSet * getMsgSet() { return &theMsgSet; }

	//add data to primary
	void addToPrimaryList(const char* str, int type, LPARAM param);
	//add data to primary
	void addToPrimaryList(TCHAR* str, int type, LPARAM param);

	//add data to secondary
	void addToSecondaryList(const char* str, int type, LPARAM param);
	void addToAttributeList(const char* key, const char* value);

private:
	//keep the layout
	int updateLayout(WPARAM wp, LPARAM lp);

	//window objects
	autownd::WndObj theMainWnd;

	ItemPanel thePrimaryPanel, theSecondaryPanel;
	AttPanel theAttPanel;
	DisplayButton theUpButton;

	////////////////////////////////////

	//message map
	WndMsgSet theMsgSet;

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
	} theLayoutMgr;

	//build the mainwnd
	int buildMainWnd();

	//str fun
	static void convertToWStr(std::wstring &dest, const std::string source);
};

