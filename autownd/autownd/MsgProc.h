#pragma once
#include "WndProgram.h"
#include <list>

namespace autownd
{
	template<class T> class MsgProc
		:public IMsgProcess
	{
	public:
		MsgProc(T* obj, int(T::*fun)(WPARAM wp, LPARAM lp)) :theObj(obj), theFun(fun) {}
		~MsgProc() {}

		int handleMsg(WPARAM wp, LPARAM lp) override {
			return (theObj->*theFun)(wp, lp);
		}
	private:
		T* theObj;
		int (T::*theFun)(WPARAM wp, LPARAM lp);
	};

	class MsgProcSet :public MsgSet {
	public:
		~MsgProcSet() {
			for (auto it : theRecycle) delete it;
		}
		template<class T> void addMsgProc(UINT msg, T* obj, int(T::*fun)(WPARAM wp, LPARAM lp)) {
			MsgProc<T>* temp = new MsgProc<T>(obj, fun);
			addMsgPair(msg, temp);
			theRecycle.push_back(temp);
		}
	private:
		std::list<IMsgProcess*> theRecycle;
	};

	class QuitMsg
		:public IMsgProcess
	{
		int handleMsg(WPARAM wp,LPARAM lp) override;
	};

	extern QuitMsg msg_quit;
}