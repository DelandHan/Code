// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\main.h"
#include <iostream>
#include <fstream>
#include<sstream>
using namespace std;



int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/*xml::XMLParser xp;

	xp.parseFile("iecu.xml");
	xml::XMLNode * docu = xp.pickupDocument();

	xml::XMLNode* node = docu;

	xml::XMLAnd aa;

	xml::XMLEqual * ee = aa.addCondition<xml::XMLEqual>();

	ee->setCondition("", xml::ELEMENT_NODE);

	while (node)
	{
		if (aa.isMatch(node))
		{
			cout << node->getString() << endl;
		}

		node = node->getNextByChain();

	}
	*/

	xml::XMLConsole cons;

	cons.setCondition("name==\"test\" AND type==ELEMENT_NODE OR name==\"disk\"");

	system("pause");
    return 0;
}

