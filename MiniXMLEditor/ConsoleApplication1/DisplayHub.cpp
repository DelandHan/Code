#include "stdafx.h"
#include "DisplayHub.h"


DisplayHub::DisplayHub()
{
}


DisplayHub::~DisplayHub()
{
}

int DisplayHub::initialize()
{
	if (theDisplayBoard.initialize()) return 1;

	theDisplayBoard.getMsgSet()->addMsgProc(WM_NOTIFY, this, &DisplayHub::beNotified);
	theDisplayBoard.getMsgSet()->addMsgProc(WM_COMMAND, this, &DisplayHub::onCommand);

	return 0;
}

int DisplayHub::beNotified(WPARAM wp, LPARAM lp)
{
	return 0;
}

int DisplayHub::onCommand(WPARAM wp, LPARAM lp)
{
	return 0;
}
