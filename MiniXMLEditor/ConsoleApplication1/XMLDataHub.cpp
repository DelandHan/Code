#include "stdafx.h"
#include "XMLDataHub.h"

using namespace xml;
using namespace std;

void convertToWStr(std::wstring &dest, const std::string &source) {
	dest.resize(source.size());
	std::copy(source.begin(), source.end(), dest.begin());

}

void convertToStr(std::string &dest, const std::wstring &source)
{
	dest.resize(source.size());
	WideCharToMultiByte(CP_ACP, 0, &source[0], (int)source.size(), &dest[0], dest.size(), 0, 0);
}

XMLDataHub::XMLDataHub()
{
}


XMLDataHub::~XMLDataHub()
{
	if (theNode) delete theNode;
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
		convertToWStr(data.str, childNode->getString());
		data.type = childNode->getType();
		data.param = (LPARAM)childNode;

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
		convertToWStr(data.key, attnode->getKey());
		convertToWStr(data.value, attnode->getValue());
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

int XMLDataHub::queryItem(ItemData * pool)
{
	XMLNode *node = nullptr;
	if (pool->param == 0) node=theNode;
	else node = (XMLNode*)pool->param;

	convertToWStr(pool->str, node->getString());
	pool->type = node->getType();

	return 0;
}

int XMLDataHub::setItem(ItemData * source)
{
	XMLNode *node = nullptr;
	if (source->param == 0) return 1;
	else node = (XMLNode*)source->param;

	if (source->str.size()) {
		string buff;
		convertToStr(buff, source->str);
		node->setString(buff);
	}

	if (source->type) {
		node->convertType(source->type ? ELEMENT_NODE : TEXT_NODE);
	}

	return 0;
}

int XMLDataHub::setItemAtt(LPARAM param, std::wstring * oldkey, std::wstring * value, std::wstring * nekey)
{
	XMLNode *node = nullptr;
	if (param == 0) return 1;
	else node = (XMLNode*)param;

	string buff_ok, buff_v, buff_k;
	convertToStr(buff_ok, *oldkey);
	convertToStr(buff_v, *value);
	convertToStr(buff_k, *nekey);

	node->setAttribute(buff_ok, buff_v, buff_k);
	return 0;
}
