// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

class Test
	:public ILine
{
public:
	Test() {
		theNode = nullptr;
	}
	~Test() {
	}

	void setNode(xml::XMLNode *node) {
		theNode = node;
	}


	int expand(Page * dest) { 	
		xml::XMLNode *node = theNode->getFirstChild();
		
		while (node)
		{
			dest->add<Test>()->setNode(node);
			node = node->getNext();
		}
		return 0;
	}
	int getMyPage(Page * dest) { return 0; }

	std::wstring getStr() { 
		std::wstring buff;
		buff.resize(theNode->getString().size());
		std::copy(theNode->getString().begin(), theNode->getString().end(), buff.begin());
		return buff; 
	}
	int getFormat() { return 0; }

private:
	xml::XMLNode * theNode;
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	xml::XMLParser ps;
	ps.parseFile("IECU.xml");
	xml::XMLNode *node = ps.pickupDocument();

	Test seed;
	seed.setNode(node);

	ILine::Page page, page2;

	seed.expand(&page);

	ILine *body = 0;
	for (ILine::Page::Range r = page.getAll(); r.first != r.second; r.first++)
	{
		std::cout << "1. ";
		std::wstring bu = (*r.first)->getStr();
		if (bu[0] == L'A') {
			body = *(r.first);
			body->expand(&page2);
		}
	}

	for (ILine::Page::Range r = page2.getAll(); r.first != r.second; r.first++)
	{
		std::wcout << (r.first).operator*()->getStr() << std::endl;
	}


	delete node;

	system("pause");
    return 0;
}

