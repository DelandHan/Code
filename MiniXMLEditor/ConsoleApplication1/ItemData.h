#pragma once
#include <string>

class LVData
{
public:
	LVData();
	LVData(std::wstring str, LPARAM param);
	~LVData();

	std::wstring strW();
	std::string str();

	void setStr(const std::string source);
	void setStr(const std::wstring source);

	int isRemove() { return !theStr.size(); }

	inline void setParam(LPARAM p) { theParam = p; }
	inline LPARAM param() { return theParam; }

	static void convertToWStr(std::wstring &dest, const std::string &source);
	static void convertToStr(std::string &dest, const std::wstring &source);

protected:
	std::wstring theStr;
	LPARAM theParam;
};

class ItemData
	:public LVData
{
public:
	ItemData();
	ItemData(std::wstring str, int type, LPARAM param);
	~ItemData();

	void setValue(const std::string value);
	void setValue(const std::wstring value);

	void beFolder();
	void beText();

private:
	int theType;
	std::wstring theValue;
};

typedef std::list<LVData> LVPool;
typedef std::list<ItemData> ItemPool;
