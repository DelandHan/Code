#pragma once

//the obj can be moved. used by UI
class IMoveableWndObj {
public:
	//drag
	virtual void drag(int x, int y, int width, int height) = 0;
};
