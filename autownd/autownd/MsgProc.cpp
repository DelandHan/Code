#include "MsgProc.h"
using namespace autownd;

int QuitMsg::handleMsg(WPARAM wp, LPARAM lp)
{
	PostQuitMessage(0);
	return 1;
}

QuitMsg autownd::msg_quit;