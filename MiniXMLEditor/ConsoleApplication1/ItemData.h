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

private:
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

	inline int type() { return theType; }
	inline void setType(int t) { theType = t; }

private:
	int theType;
};

class AttData
	:public LVData
{
public:
	AttData();
	~AttData();

	std::wstring valueW();
	std::string value();

	void setValue(const std::string source);
	void setValue(const std::wstring source);

private:
	std::wstring theValue;

};

typedef std::list<LVData> LVPool;
typedef std::list<ItemData> ItemPool;
typedef std::list<AttData> AttPool;