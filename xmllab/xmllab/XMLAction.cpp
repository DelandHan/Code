#include "XMLAction.h"

using namespace xml;
using namespace action;

xml::XMLNode * xml::action::XMLDel::handle(xml::XMLNode * node)
{
	if (node == nullptr) return nullptr;

	XMLNode * temp = node->getNextByChain(false);
	delete node;
	return temp;
}

xml::XMLNode * xml::action::XMLRemove::handle(xml::XMLNode * node)
{
	XMLNode * temp = node->getLastChild();

	while (temp)
	{
		XMLNode * pre = temp->getPrevious();
		temp->removeMe();
		node->insert(temp, true);
		temp = pre;
	}

	temp = node->getNext();
	delete node;
	return temp;
}

xml::action::XMLReplace::XMLReplace()
	:theType(GENERAL_NODE)
{
}

xml::XMLNode * xml::action::XMLReplace::handle(xml::XMLNode * node)
{
	if (theStr.size()) node->setString(theStr);
	if (theType != GENERAL_NODE) node->convertType(theType);
	return node;
}

void xml::action::XMLReplace::setNewStatus(std::string str, NodeType type)
{
	theStr = str;
	theType = type;
}
