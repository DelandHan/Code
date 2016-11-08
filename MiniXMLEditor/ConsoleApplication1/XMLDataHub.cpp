#include "stdafx.h"
#include "XMLDataHub.h"

using namespace xml;
using namespace std;

XMLDataHub::XMLDataHub()
{
	theNode = new XMLNode(DOCUMENT_NODE);
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

int XMLDataHub::getItemAtt(LPARAM param, ItemPool * pool)
{
	XMLNode *node = nullptr;
	if (param == 0) return 1;
	else node = (XMLNode*)param;

	if (node->getType() != ELEMENT_NODE) return 1;

	const XMLNode::AttNode *attnode = nullptr;

	while (attnode = node->getAttribute(attnode))
	{
		pool->emplace_back();
		ItemData &data = pool->back();
		string str;
		str.reserve(attnode->getKey().size() + attnode->getValue().size() + 3);
		str += attnode->getKey();
		str += "=\"";
		str += attnode->getValue();
		str += "\"";
		data.setStr(str);
		data.setType(NodeType::ATTRIBUTE_NODE);
		data.setParam((LPARAM)node);
	}

	return 0;
}

LPARAM XMLDataHub::queryParent(LPARAM param)
{
	XMLNode *node = nullptr;
	if (param == 0) return (LPARAM)theNode;
	else node = (XMLNode*)param;

	LPARAM parent = (LPARAM)node->getParent();
	return parent == 0 ? (LPARAM)theNode : parent;
}

void XMLDataHub::loadFile(const char * name)
{
	XMLParser xp;
	xp.parseFile(name);
	if (theNode) delete theNode;

	theNode = xp.pickupDocument();
}

int XMLDataHub::queryItem(ItemData * data)
{
	XMLNode *node = nullptr;
	if (data->param() == 0) node = theNode;
	else node = (XMLNode*)data->param();

	data->setStr(node->getString());
	data->setType(node->getType());
	data->setParam((LPARAM)node);

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
		node->convertType((NodeType)source->type());
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

std::string XMLDataHub::parse(ItemPool * data)
{
	string buff;
	XMLParser xp;

	for (ItemPool::iterator it = data->begin(); it != data->end(); it++)
	{
		XMLNode * node = (XMLNode *)it->param();
		xp.saveNode(node, &buff);
	}

	return buff;
}

int XMLDataHub::addItem(LPARAM param, std::string text, DOMPos pos, ItemPool *record)
{
	XMLNode *node = (XMLNode*)param;
	if (node == nullptr) return 1;

	XMLParser ps;
	ps.parse(&text[0], text.size(), true);
	XMLNode * nenode = ps.pickupDocument();

	XMLNode * temp = nenode->getFirstChild();
	while (temp)
	{
		XMLNode *toins = temp;
		temp = temp->getNext();
		toins->removeMe();
		switch (pos)
		{
		case IDataHub::BEFORE:
			node->insert(toins, false);
			break;
		case IDataHub::AFTER:
			node->insert(toins, true);
			node = toins;
			break;
		case IDataHub::CHILD:
			node->append(toins);
			break;
		default:
			continue;
		}

		if (record)
		{
			record->emplace_back(); ItemData & item = record->back();
			item.setStr(toins->getString());
			item.setType(toins->getType());
			item.setParam((LPARAM)toins);
		}
	}
	delete nenode;

	return 0;
}

int XMLDataHub::inputCmd(LPARAM param, std::string cmd)
{
	XMLNode *node = (XMLNode*)param;
	if (node == nullptr) return 1;

	size_t sym = cmd.find("WHERE");

	if (sym == string::npos) return 1;

	XMLConsole console;

	if (console.addAction(string(cmd.c_str(), sym))) return 1;
	if (console.setCondition(string(cmd.c_str() + sym + 6, cmd.size() - sym - 6))) return 1;
	console.connectTo(node);

	return console.run();

}

int XMLDataHub::reset()
{
	if (theNode) delete theNode;
	theNode = new XMLNode(DOCUMENT_NODE);
	return 0;
}
