#pragma once
#include "WndProgram.h"

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

	class QuitMsg
		:public IMsgProcess
	{
		int handleMsg(WPARAM wp,LPARAM lp) override;
	};

	extern QuitMsg msg_quit;
}