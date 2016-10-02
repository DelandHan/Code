#pragma once
#include <string>
#include <map>
#include "Bullet.h"
#include <Windows.h>

namespace autownd
{
	int msgLoop();

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
		inline void show() { ShowWindow(theWnd, SW_SHOW); }

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
		virtual int handleMsg(memory::ParamChain params) = 0;
	};

	////////

	class MsgSet
	{
	public:
		MsgSet();
		template<class T> MsgSet(std::pair<UINT, T>* begin, size_t size) {
			addMsgPairs(begin, size);
		}
		~MsgSet();
		IMsgProcess *retrieve(UINT msg) const;
		void addMsgPair(UINT msg, IMsgProcess* proc);
		template<class T> void addMsgPairs(std::pair<UINT, T>* begin, size_t size) {
			for (size_t i = 0; i < size; i++) {
				theMap.insert(std::pair<UINT, IMsgProcess*>((begin + i)->first, &(begin + i)->second));
			}
		}
	private:
		std::map<UINT, IMsgProcess*> theMap;
	};

	//use to create Wnds
	class Seed
	{
	public:
		Seed();
		~Seed();
		void init(memory::ParamChain params);

		int create(WndObj *obj, const MsgSet *msgmap, memory::ParamChain params);

	private:
		std::wstring theName;
		
		static const MsgSet* theAddingMsgs;
		static WndObj* theAddingObj;

		static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	};

}
