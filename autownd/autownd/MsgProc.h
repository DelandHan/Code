#pragma once
#include "WndProgram.h"

namespace autownd
{
	template<class T> class MsgProc
		:public IMsgProcess
	{
	public:
		MsgProc(T* obj, int(T::*fun)(WndObj *, WPARAM, LPARAM)) :theObj(obj), theFun(fun) {}
		~MsgProc() {}

		int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override {
			return (theObj->*theFun)(obj, wp, lp);
		}
	private:
		T* theObj;
		int (T::*theFun)(WndObj *, WPARAM, LPARAM);
	};

	class QuitMsg
		:public IMsgProcess
	{
		int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override;
	};

	extern QuitMsg msg_quit;
}