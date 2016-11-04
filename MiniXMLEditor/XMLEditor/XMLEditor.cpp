// XMLEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WndBoard.h"
#include "TreePanel.h"

int main()
{
	TreePanel tp;

	tp.initialize();

	xml::XMLParser xp;
	xp.parseFile("doc.xml");
	xml::XMLNode *node = xp.pickupDocument();

	tp.setXMLNode(node);

	autownd::msgLoop();

    return 0;
}

