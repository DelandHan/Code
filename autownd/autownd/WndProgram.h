#pragma once
#include <string>
#include <map>
#include <list>
#include "Bullet.h"
#include <Windows.h>

namespace autownd
{
	int msgLoop(std::pair<HWND, int> acc = { 0,0 });

	typedef std::pair<int, int> vec;

	class WndObj;
	class IMsgProcess;
	class MsgSet;

	class WndObj
	{
	public:
		WndObj();
		virtual ~WndObj();

		inline HWND wnd() { return theWnd; }
		inline void show(int cmd = SW_SHOW) { ShowWindow(theWnd, cmd); }

		int addControl(WndObj* obj, TCHAR * cname, memory::ParamChain params);

	private:
		friend class Seed;
		HWND theWnd;

		static std::map<HWND, const MsgSet *> theWndMap;
	};

	//make the process
	class IMsgProcess
	{
	public:
		virtual ~IMsgProcess() {}//make sure the dervid class's decon is called.
		virtual int handleMsg(WPARAM wp, LPARAM lp) = 0;
	};

	////////

	class MsgSet
	{
	public:
		MsgSet() {}
		~MsgSet() {}
		IMsgProcess *retrieve(UINT msg) const;
		void addMsgPair(UINT msg, IMsgProcess* proc);

	private:
		std::map<UINT, IMsgProcess*> theMap;
	};

	//use to create Wnds
	class Seed
	{
	public:
		Seed() {}
		~Seed() {}
		void init(memory::ParamChain params);

		int create(WndObj *obj, const MsgSet *msgmap, memory::ParamChain params);

	private:
		std::wstring theName;
		
		static const MsgSet* theAddingMsgs;
		static WndObj* theAddingObj;

		static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	};

}
