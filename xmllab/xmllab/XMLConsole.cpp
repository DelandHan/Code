#include "XMLConsole.h"
#include <sstream>

using namespace xml;
using namespace xml::condition;
using namespace xml::action;
using namespace std;

XMLConsole::XMLConsole()
	:theNode(nullptr), theRoot(nullptr)
{
}


XMLConsole::~XMLConsole()
{
	delete theRoot;
	for (auto it : theActions) delete it;
}

int xml::XMLConsole::setCondition(std::string cmd)
{
	theRoot = getContainer(cmd);
	if (theRoot == nullptr) return 1;
	else return 0;
}

int xml::XMLConsole::addAction(std::string cmd)
{
	theActions.push_back(strMapAction(cmd));
	return 0;
}

int xml::XMLConsole::run()
{
	XMLNode * end = theNode->getNextByChain(false);
	XMLNode * node = theNode->getNextByChain();

	while (node != end)
	{
		XMLNode * temp = nullptr;
		if (theRoot->isMatch(node))
		{
			for (auto it : theActions)
			{
				temp = it->handle(node);
				if (temp != node) break;
			}
			if (temp) node = temp;
			else node = node->getNextByChain();
		}
		else
			node = node->getNextByChain();
	}

	return 0;
}

XMLLogic * xml::XMLConsole::getContainer(std::string cmd)
{
	std::istringstream iss(cmd);
	std::string sub1, sub2;

	XMLLogic * result = nullptr;
	IXMLCondition* firstCon = nullptr;

	std::getline(iss, sub1, ' ');
	firstCon = strMapCondition(sub1);

	do
	{
		std::getline(iss, sub1, ' ');
		std::getline(iss, sub2, ' ');

		XMLLogic * container = strMapContainer(sub1);
		IXMLCondition * con = strMapCondition(sub2);

		if (container == nullptr || con == nullptr) continue; //maybe error

		if (result == nullptr)
		{
			result = container;
			if (firstCon) result->addCondition(firstCon);
			firstCon = nullptr;
			result->addCondition(con);
		}
		else if (result->getString() == container->getString())
		{
			result->addCondition(con);
			delete container;
		}
		else
		{
			container->addCondition(result);
			container->addCondition(con);
			result = container;
		}
	} while (!iss.eof());

	return result;
}

IXMLCondition * xml::XMLConsole::strMapCondition(std::string cmd)
{
	size_t sym = string::npos;

	if ((sym = cmd.find("==")) != string::npos) //find ==
	{
		XMLEqual * eq = new XMLEqual;
		string name; NodeType type = NodeType::GENERAL_NODE;

		if (!memcmp(cmd.c_str(), "name", 4)) 
		{
			size_t endstr = cmd.find('\"', sym + 3);
			name.assign(cmd.c_str() + sym + 3, endstr -sym - 3);
		}
		if (!memcmp(cmd.c_str(), "type", 4))
		{
			type = getTypeByStr(cmd.c_str() + 6);
		}

		eq->setCondition(name, type);

		return eq;
	}
	return nullptr;
}

XMLLogic * xml::XMLConsole::strMapContainer(std::string cmd)
{
	if (cmd == "AND") return new XMLAnd;
	if (cmd == "OR") return new XMLOr;
	return nullptr;
}

action::IXMLAction * xml::XMLConsole::strMapAction(std::string cmd)
{
	if (cmd == "DEL") return new XMLDel;
	if (cmd == "REMOVE") return new XMLRemove;

	return nullptr;
}

