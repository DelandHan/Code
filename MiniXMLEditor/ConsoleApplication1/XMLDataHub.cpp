#include "stdafx.h"
#include "XMLDataHub.h"

using namespace xml;
using namespace std;

void convertToWStr(std::wstring &dest, const std::string &source) {
	dest.resize(source.size());
	std::copy(source.begin(), source.end(), dest.begin());

}

void convertToStr(std::string &dest, TCHAR * source)
{
	std::wstring buff = source;
	dest.resize(buff.size());
	WideCharToMultiByte(CP_ACP, 0, &buff[0], (int)buff.size(), &dest[0], dest.size(), 0, 0);
}

XMLDataHub::XMLDataHub()
{
}


XMLDataHub::~XMLDataHub()
{
}

int XMLDataHub::getChildItemData(LPARAM param, ItemPool * pool)
{
	XMLNode *node = nullptr;
	if (param == 0) node = theNode;
	else node = (XMLNode*)param;

	if (node->getType() != ELEMENT_NODE && node->getType() != DOCUMENT_NODE) return 1;

	XMLNode * childNode = node->getFirstChild();

	while (childNode)
	{
		pool->emplace_back();
		ItemData &data = pool->back();
		convertToWStr(data.first.first, childNode->getString());
		data.first.second = childNode->getType();
		data.second = (LPARAM)childNode;

		childNode = childNode->getNext();
	}

	return 0;
}

int XMLDataHub::getItemAtt(LPARAM param, AttPool * pool)
{
	XMLNode *node = nullptr;
	if (param == 0) return 1;
	else node = (XMLNode*)param;

	if (node->getType() != ELEMENT_NODE) return 1;

	const XMLNode::AttNode *attnode = nullptr;

	while (attnode = node->getAttribute(attnode))
	{
		pool->emplace_back();
		AttData &data = pool->back();
		convertToWStr(data.first, attnode->getKey());
		convertToWStr(data.second, attnode->getValue());
	}

	return 0;
}

LPARAM XMLDataHub::queryParent(LPARAM param)
{
	XMLNode *node = nullptr;
	if (param == 0) return 0;
	else node = (XMLNode*)param;
	return (LPARAM)node->getParent();
}

void XMLDataHub::loadFile(const char * name)
{
	XMLParser xp;
	xp.parseFile(name);
	if (theNode) delete theNode;

	theNode = xp.pickupDocument();
}
