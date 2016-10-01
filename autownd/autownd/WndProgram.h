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

		IMsgProcess* getMsgProc(UINT msg);
		void setMsgSet(const MsgSet *set);

	private:
		friend class Seed;
		const MsgSet *theMsgMap;
		HWND theWnd;

		static std::map<HWND, WndObj*> theWndMap;
	};

	//make the process
	class IMsgProcess
	{
	public:
		virtual int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) = 0;
	};

	////////

	class MsgSet
	{
	public:
		MsgSet();
		template<class T> MsgSet(std::pair<UINT, T>* begin, std::pair<UINT, T>* end) {
			addMsgPairs(begin, end);
		}
		~MsgSet();
		IMsgProcess *retrieve(UINT msg) const;
		void addMsgPair(UINT msg, IMsgProcess* proc);
		template<class T> void addMsgPairs(std::pair<UINT, T>* begin, std::pair<UINT, T>* end) {
			std::pair<UINT, T>* it = begin;
			while (1) {
				theMap.insert(std::pair<UINT, IMsgProcess*>(it->first, &it->second));
				if (it == end) return;
				it++;				
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

		int create(WndObj *obj, memory::ParamChain params);
		int create(WndObj *obj, int resourceid);

	private:
		std::wstring theName;
		
		static WndObj* theAdding;
		static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
		static INT_PTR CALLBACK DialProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	};

}
