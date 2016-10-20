#pragma once
#include <vector>
#include <list>

class ItemData
{
public:
	ItemData(int i);
	~ItemData();

	void setStr(int id, std::string & str);
	void setStrW(int id, std::wstring & str);

	std::string getStr(int id);
	std::wstring getStrW(int id);

private:
	std::vector<std::wstring> theStrLst;
	LPARAM theParam;

	void convertToWStr(std::wstring &dest, const std::string &source);
	void convertToStr(std::string &dest, const std::wstring &source);

};

typedef std::list<ItemData> ItemPool;