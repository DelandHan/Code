#pragma once
#include <list>

class WndDisplay
{
public:
	WndDisplay();
	~WndDisplay();

	//initialize
	int initialize();

	//get the msgmap
	autownd::MsgSet * getMap();

	//keep the layout
	int updateLayout(memory::ParamChain params);

private:
	////////////////classes//////////////////
	class DisplayObj {
	public:
		virtual ~DisplayObj() {}
		
		//set the ratio of size and pos. the max is 100
		void setRect(autownd::vec &size, autownd::vec &pos);
		//move the panel base on the ratio
		void move(const RECT * clientRect);

		//init the panel. provided by the derive class
		virtual int initialize(autownd::WndObj *parent) = 0;
		//get hwnd
		virtual HWND wnd() = 0;
	private:
		autownd::vec theSize, thePos;

		//when the wnd size changed
		virtual int onSize(int width, int height) = 0;
	};

	class ItemPanel :public DisplayObj {
	public:
		int initialize(autownd::WndObj *parent) override;
		HWND wnd() override { return theObj.wnd(); }
	private:
		int onSize(int width, int height) override;
		autownd::List theObj;
	};

	class AttPanel :public DisplayObj {
	public:
		int initialize(autownd::WndObj *parent) override;
		HWND wnd() override { return theObj.wnd(); }
	private:
		int onSize(int width, int height) override;
		autownd::List theObj;
	};

	class DisplayButton :public DisplayObj {
	public:
		int initialize(autownd::WndObj *parent) override;
		HWND wnd() override { return theObj.wnd(); }
	private:
		int onSize(int width, int height) override;
		autownd::WndObj theObj;
	};

	////////////////members////////////////
	typedef std::list<DisplayObj*> ObjectList;
	ObjectList theObjLst;

	//window objects
	autownd::WndObj theMainWnd;

	//message map
	autownd::MsgSet theMsgMap;

	////////////////internal fun////////////////
	void addObj(DisplayObj* obj, autownd::vec size, autownd::vec pos);
	int buildMainWnd();
};

