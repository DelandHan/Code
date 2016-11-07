#include "XMLConsole.h"
#include<sstream>

using namespace xml;

XMLConsole::XMLConsole()
	:theNode(nullptr), theCondition(nullptr)
{
}


XMLConsole::~XMLConsole()
{
	delete theCondition;
}

int xml::XMLConsole::setCondition(std::string cmd)
{
	std::istringstream iss(cmd);
	std::string sub1, sub2;

	IXMLCondition* start = nullptr;

	std::getline(iss, sub1, ' ');
	start = strMapCondition(sub1);

	do
	{
		std::getline(iss, sub1, ' ');
		std::getline(iss, sub2, ' ');

		XMLConditionContainer * container = strMapContainer(sub1);
		IXMLCondition * con = strMapCondition(sub2);

		if (container == nullptr || con == nullptr) continue;

		if (theCondition == nullptr)
		{
			theCondition = container;
			if (start) theCondition->addCondition(start);
			start = nullptr;
			theCondition->addCondition(con);
		}
		else if (theCondition->getString() == container->getString())
		{
			theCondition->addCondition(con);
			delete container;
		}
		else
		{
			container->addCondition(theCondition);
			container->addCondition(con);
			theCondition = container;
		}

	} while (!iss.eof());

	return 0;
}

int xml::XMLConsole::setAction(std::string cmd)
{
	return 0;
}

int xml::XMLConsole::run()
{
	return 0;
}

IXMLCondition * xml::XMLConsole::strMapCondition(std::string cmd1)
{
	XMLEqual *eq = new XMLEqual;
	eq->setCondition(cmd1, GENERAL_NODE);
	return eq;
}

XMLConditionContainer * xml::XMLConsole::strMapContainer(std::string cmd)
{
	if (cmd == "AND") return new XMLAnd;
	if (cmd == "OR") return new XMLOr;
	return nullptr;
}

