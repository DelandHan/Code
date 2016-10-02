#pragma once
#include "WndProgram.h"

namespace autownd
{
	template<class T> class MsgProc
		:public IMsgProcess
	{
	public:
		MsgProc(T* obj, int(T::*fun)(memory::ParamChain params)) :theObj(obj), theFun(fun) {}
		~MsgProc() {}

		int handleMsg(memory::ParamChain params) override {
			return (theObj->*theFun)(params);
		}
	private:
		T* theObj;
		int (T::*theFun)(memory::ParamChain params);
	};

	class QuitMsg
		:public IMsgProcess
	{
		int handleMsg(memory::ParamChain params) override;
	};

	extern QuitMsg msg_quit;
}