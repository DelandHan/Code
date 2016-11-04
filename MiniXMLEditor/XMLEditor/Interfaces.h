#pragma once

//the obj can be moved. used by UI
class IMoveableWndObj
{
public:
	virtual int initialize(autownd::WndObj *parent) = 0;
	//drag
	virtual void drag(int x, int y, int width, int height) = 0;
};

