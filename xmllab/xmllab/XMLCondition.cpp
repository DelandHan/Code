#include "XMLCondition.h"

using namespace xml;
using namespace std;

XMLConditionContainer::~XMLConditionContainer()
{
	for (auto it : theChilds) delete it;
}

void xml::XMLConditionContainer::addCondition(IXMLCondition * con)
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

xml::XMLEqual::XMLEqual()
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
