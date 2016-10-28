#pragma once
#include <string>
#include <list>

//the obj can be moved. used by UI
class IMoveableWndObj
{
public:
	virtual int initialize(autownd::WndObj *parent) = 0;
	//drag
	virtual void drag(int x, int y, int width, int height) = 0;
};

class ILine
{
public:
	virtual ~ILine() = default;

	typedef std::list<ILine*> Page;
	
	virtual int getMyPage(Page * dest) = 0;
	virtual int expandMe(Page * dest) = 0;

	virtual std::wstring getStr() = 0;
	virtual int getFormat() = 0;

};