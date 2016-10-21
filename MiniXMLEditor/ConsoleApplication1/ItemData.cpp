#include "stdafx.h"
#include "ItemData.h"

using namespace std;
/*
ItemData::ItemData(int i)
	: theParam(0), theType(i)
{
	if (i == 2) theStrLst.resize(2);
	else theStrLst.resize(1);
}

ItemData::~ItemData()
{
}

void ItemData::setStr(int id, std::string & str)
{
	convertToWStr(theStrLst[id], str);
}

void ItemData::setStrW(int id, std::wstring & str)
{
	theStrLst[id] = str;
}

std::string ItemData::getStr(int id)
{
	string data;
	convertToStr(data, theStrLst[id]);
	return std::move(data);
}

std::wstring ItemData::getStrW(int id)
{
	return theStrLst[id];
}

void ItemData::convertToWStr(std::wstring & dest, const std::string & source)
{
	dest.resize(source.size());
	std::copy(source.begin(), source.end(), dest.begin());
}

void ItemData::convertToStr(std::string & dest, const std::wstring & source)
{
	dest.resize(source.size());
	WideCharToMultiByte(CP_ACP, 0, &source[0], (int)source.size(), &dest[0], dest.size(), 0, 0);
}
*/