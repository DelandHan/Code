#pragma once
#include "XMLNode.h"
#include "XMLCondition.h"
namespace xml
{

	class XMLConsole
	{
	public:
		XMLConsole();
		~XMLConsole();

		void connectTo(XMLNode* node) { theNode = node; }

		int setCondition(std::string cmd);
		int setAction(std::string cmd);

		int run();

	private:
		XMLNode * theNode;
		XMLConditionContainer *theCondition;
		
		IXMLCondition * strMapCondition(std::string cmd);
		XMLConditionContainer * strMapContainer(std::string cmd);

	};

}
