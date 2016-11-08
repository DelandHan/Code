#include "XMLCondition.h"

using namespace xml;
using namespace xml::condition;
using namespace std;

XMLLogic::~XMLLogic()
{
	for (auto it : theChilds) delete it;
}

void XMLLogic::addCondition(IXMLCondition * con)
{
	theChilds.push_back(con);
}

int XMLAnd::isMatch(XMLNode * node)
{
	int result = 1;

	for (auto it : theChilds)
	{
		result = result && it->isMatch(node);
	}

	return result;
}

int XMLOr::isMatch(XMLNode * node)
{
	int result = 0;

	for (auto it : theChilds)
	{
		result = result || it->isMatch(node);
	}

	return result;
}

XMLEqual::XMLEqual()
	:theType(NodeType::GENERAL_NODE)
{
}

int XMLEqual::isMatch(XMLNode * node)
{
	int result = 1;

	if (theStr != "") result = result && (theStr == node->getString());

	if (theType!= GENERAL_NODE) result = result && (theType == node->getType());

	return result;
}

void XMLEqual::setCondition(std::string str, NodeType type)
{
	theStr = str;
	theType = type;
}

xml::condition::XMLChildContain::XMLChildContain()
	:theChildCondition(nullptr)
{
}

int xml::condition::XMLChildContain::isMatch(XMLNode * node)
{
	int result = 0;
	if (theChildCondition == nullptr) return 0;

	XMLNode *child = node->getFirstChild();

	while (child)
	{
		result = result || theChildCondition->isMatch(child);
		child = child->getNext();
	}

	return result;
}
