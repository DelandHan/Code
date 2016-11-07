#pragma once
#include "XMLNode.h"
#include "XMLCondition.h"
#include "XMLAction.h"
#include <list>

namespace xml
{
	class XMLConsole
	{
	public:
		XMLConsole();
		~XMLConsole();

		void connectTo(XMLNode* node) { theNode = node; }

		int setCondition(std::string cmd);
		int addAction(std::string cmd);

		int run();

	private:
		XMLNode * theNode;

		condition::XMLLogic *theRoot;
		condition::XMLLogic *getContainer(std::string cmd);		
		condition::IXMLCondition * strMapCondition(std::string cmd);
		condition::XMLLogic * strMapContainer(std::string cmd);

		std::list<action::IXMLAction *> theActions;
		action::IXMLAction * strMapAction(std::string cmd);
	};

}
