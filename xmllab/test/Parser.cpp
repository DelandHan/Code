#include "stdafx.h"
#include "Parser.h"

using namespace parser;
using namespace std;

parser::Chain::Chain()
	:theStart(nullptr), theEnd(nullptr), thePos(nullptr)
{
}

parser::Chain::Chain(const char * start, size_t count)
{
	assign(start, count);
}

parser::Chain::Chain(const Chain & other)
	: theStart(other.theStart), theEnd(other.theEnd), thePos(other.thePos)
{
}

parser::Chain::~Chain()
{
}

void parser::Chain::assign(const char * source, size_t count)
{
	theStart = source;
	theEnd = theStart + count - 1;
	thePos = theStart;
}

void parser::Chain::cut(const char * c)
{
	if (c >= theStart && c <= theEnd)
	{
		theStart = c;
		if (thePos < c) thePos = c;
	}
}

parser::ChainChecker::ChainChecker()
{
	theIndex.reserve(5);
}

parser::ChainChecker::~ChainChecker()
{
}

int parser::ChainChecker::check(Chain * inchain, Chain * subchain)
{
	const char *pos = nullptr;
	while (pos = inchain->pop())
	{
		size_t index;
		if ((index = firstcheck(*pos)) != string::npos) //maybe change string::npos?
		{
			int result = match(inchain, index);
			if (result >= 0)
			{
				subchain->assign(inchain->getStart(), inchain->prefix(pos));
				inchain->cut(inchain->get());
				return result;
			}
		}
	}

	return -1;
}

int parser::ChainChecker::addEndStr(std::string end_str)
{
	size_t index;
	if ((index = theIndex.find(end_str.front())) == string::npos)
	{
		theIndex.push_back(end_str.front());
		index = theIndex.size() - 1;
		theTokenIndex.resize(index + 1);
	}

	theTokenIndex[index].push_back(end_str.c_str() + 1);

	return index * 1000 + theTokenIndex[index].size() - 1; //need a function or macro
}

int parser::ChainChecker::match(Chain * oncheck, size_t index)
{
	size_t i = 0;
	for (; i < theTokenIndex[index].size(); i++)
	{
		string &str = theTokenIndex[index][i];
		if (static_cast<size_t>(oncheck->suffix(oncheck->get())) < str.size()) continue;//too less data remain
		if (!memcmp(str.c_str(), oncheck->get(), str.size())) //match! 
		{
			oncheck->move(str.size());
			return index * 1000 + i; //need a function or macro
		}
	}
	return -1; //not match
}
