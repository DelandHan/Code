#include "XMLConsole.h"
#include <sstream>
#include <iterator>

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
	if (theRoot) delete theRoot;

	std::istringstream iss(cmd);

	list<string> tokens{ istream_iterator<string>{iss},istream_iterator<string>{} };

	theRoot = getContainer(tokens);
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
		if (theRoot->isMatch(node))
		{
			XMLNode * temp = nullptr;
			for (auto it : theActions)
			{
				temp = it->handle(node);
				if (temp != node) break;
			}
			if (temp != node)
			{
				node = temp;
			}
			else 
				node = node->getNextByChain();
		}
		else
			node = node->getNextByChain();
	}

	return 0;
}

IXMLCondition * xml::XMLConsole::getContainer(list<string> & tokens)
{
	XMLLogic * result = nullptr;
	IXMLCondition* firstCon = nullptr;

	firstCon = strMapCondition(*tokens.begin());
	tokens.pop_front();

	bool endbracket = false;

	while (tokens.size())
	{
		XMLLogic * container = strMapContainer(*tokens.begin());
		tokens.pop_front();

		list<string>::iterator it = tokens.begin();

		if ((*it)[it->size() - 1] == ')')
		{
			endbracket = true;
			(*it)[it->size() - 1] = 0;
		}


		IXMLCondition * con = nullptr;
		if ((*it)[0] == '(')
		{
			it->assign(it->c_str() + 1);
			con = getContainer(tokens);
		}
		else
		{
			con = strMapCondition(*it);
			tokens.pop_front();
		}

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

		if (endbracket) return result;
	}

	if (result == nullptr && firstCon) return firstCon;

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
	std::istringstream iss(cmd);
	list<string> tokens{ istream_iterator<string>{iss},istream_iterator<string>{} };

	list<string>::iterator it = tokens.begin();

	if (*it == "DEL") return new XMLDel;
	if (*it == "REMOVE") return new XMLRemove;
	if (*it == "SET")
	{
		XMLReplace *replace = new XMLReplace;
		string str; NodeType type = GENERAL_NODE;

		for (it++; it != tokens.end(); it++)
		{
			size_t sym = it->find('=');
			if (!memcmp(it->c_str(), "name", 4))
			{
				str.assign(it->c_str() + 6, it->size() - 7);
			}
			if (!memcmp(it->c_str(), "type", 4))
			{
				type = xml::getTypeByStr(it->c_str() + 5);
			}
		}
		replace->setNewStatus(str, type);
		return replace;
	}

	return nullptr;
}

