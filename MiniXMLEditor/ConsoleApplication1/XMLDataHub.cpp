#include "stdafx.h"
#include "XMLDataHub.h"

using namespace xml;
using namespace std;

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
	if (param == 0) return 1;
	else node = (XMLNode*)param;

	if (node->getType() != ELEMENT_NODE && node->getType() != DOCUMENT_NODE) return 1;

	XMLNode * childNode = node->getFirstChild();

	while (childNode)
	{
		pool->emplace_back();
		ItemData &data = pool->back();
		data.setStr(childNode->getString());
		data.setType(childNode->getType());
		data.setParam((LPARAM)childNode);

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
		data.setStr(attnode->getKey());
		data.setValue(attnode->getValue());
	}

	return 0;
}

LPARAM XMLDataHub::queryParent(LPARAM param)
{
	XMLNode *node = nullptr;
	if (param == 0) return (LPARAM)theNode;
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
	if (pool->param() == 0) node = theNode;
	else node = (XMLNode*)pool->param();

	pool->setStr(node->getString());
	pool->setType(node->getType());
	pool->setParam((LPARAM)node);

	return 0;
}

int XMLDataHub::setItem(ItemData * source)
{
	XMLNode *node = nullptr;
	if (source->param() == 0) return 1;
	else node = (XMLNode*)source->param();

	if (source->isRemove()) {
		delete node;
		return 0;
	}

	node->setString(source->str());

	if (source->type()) {
		node->convertType(source->type() ? ELEMENT_NODE : TEXT_NODE);
	}

	return 0;
}

int XMLDataHub::queryPath(LPARAM param, std::wstring &path)
{
	XMLNode *node;
	if (param == 0) node = theNode;
	else node = (XMLNode*)param;

	if (node == nullptr) return 1;

	//get path
	XMLNode *child_n = node; size_t size = 0; string buff;
	while (child_n) {
		size += child_n->getString().size() + 1;
		child_n = child_n->getParent();
	}
	buff.resize(size + 1);

	child_n = node;
	while (child_n) {
		memcpy(&buff[size - child_n->getString().size()], child_n->getString().c_str(), child_n->getString().size());
		buff[size - child_n->getString().size() - 1] = '\\';
		size -= child_n->getString().size() + 1;
		child_n = child_n->getParent();
	}

	LVData::convertToWStr(path, buff);

	return 0;
}

int XMLDataHub::setItemAtt(LPARAM param, std::wstring oldkey, std::wstring value, std::wstring nekey)
{
	XMLNode *node = nullptr;
	if (param == 0) return 1;
	else node = (XMLNode*)param;

	string buff_ok, buff_v, buff_k;
	LVData::convertToStr(buff_ok, oldkey);
	LVData::convertToStr(buff_v, value);
	LVData::convertToStr(buff_k, nekey);

	node->setAttribute(buff_ok, buff_k, buff_v);
	return 0;
}
