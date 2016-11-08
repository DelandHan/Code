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

	xml::XMLParser xp;

	xp.parseFile("iecu.xml");
	xml::XMLNode * docu = xp.pickupDocument();
	
	xml::XMLConsole cons;

	cons.setCondition("name==\"para\" AND CONTAIN (name==\"JScript\" AND type==TEXT_NODE)");// OR name==\"list\" AND type==TEXT_NODE
	cons.addAction("SET name=\"stop\" type=TEXT_NODE");
	cons.connectTo(docu);
	cons.run();
	
	ofstream of;
	of.open("out.xml", ios::binary);
	xp.saveNode(docu, &of);

	delete docu;
	system("pause");
    return 0;
}

