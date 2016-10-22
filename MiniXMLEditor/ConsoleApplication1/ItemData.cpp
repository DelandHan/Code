#include "stdafx.h"
#include "ItemData.h"

using namespace std;

LVData::LVData()
	:theParam(0)
{
}

LVData::LVData(std::wstring str, LPARAM param)
	: theStr(str), theParam(param)
{
}

LVData::~LVData()
{
}

std::wstring LVData::strW()
{
	return theStr;
}

std::string LVData::str()
{
	string buff;
	convertToStr(buff, theStr);
	return move(buff);
}

void LVData::setStr(const std::string source)
{
	convertToWStr(theStr, source);
}

void LVData::setStr(const std::wstring source)
{
	theStr = source;
}

void LVData::convertToWStr(std::wstring & dest, const std::string & source)
{
	dest.resize(source.size());
	std::copy(source.begin(), source.end(), dest.begin());
}

void LVData::convertToStr(std::string & dest, const std::wstring & source)
{
	dest.resize(source.size());
	WideCharToMultiByte(CP_ACP, 0, &source[0], (int)source.size(), &dest[0], dest.size(), 0, 0);
}

ItemData::ItemData()
	:theType(0)
{
}

ItemData::ItemData(std::wstring str, int type, LPARAM param)
	: LVData(str, param), theType(type)
{
}

ItemData::~ItemData()
{
}

AttData::AttData()
{
}

AttData::~AttData()
{
}

std::wstring AttData::valueW()
{
	return theValue;
}

std::string AttData::value()
{
	string buff;
	convertToStr(buff, theValue);
	return move(buff);
}

void AttData::setValue(const std::string source)
{
	convertToWStr(theValue, source);
}

void AttData::setValue(const std::wstring source)
{
	theValue = source;
}
