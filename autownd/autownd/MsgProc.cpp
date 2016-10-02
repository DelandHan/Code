#include "MsgProc.h"
using namespace autownd;

int QuitMsg::handleMsg(memory::ParamChain params)
{
	PostQuitMessage(0);
	return 1;
}

QuitMsg autownd::msg_quit;