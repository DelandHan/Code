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

	class Page
	{
	public:
		typedef std::pair<std::list<ILine*>::iterator, std::list<ILine*>::iterator> Range;

		Page() = default;
		~Page() { clear(); }

		void clear() {
			for (auto it : thePage) delete it;
			thePage.clear();
		}

		Range getAll() { return Range(thePage.begin(), thePage.end()); }

		template<typename T> 
		typename std::enable_if<std::is_base_of<ILine,T>::value, T*>::type add() {
			T * fresh = new T;
			thePage.push_back(fresh);
			return fresh;
		}

	private:
		std::list<ILine*> thePage;
	};
	
	virtual int expand(Page * dest) = 0;
	virtual int getMyPage(Page * dest) = 0;

	virtual std::wstring getStr() = 0;
	virtual int getFormat() = 0;

};