#include "stdafx.h"


#include <iostream>
#include "resource.h"
using namespace std;
using namespace autownd;

void convertToWStr(std::wstring &dest, const std::string &source) {
	dest.resize(source.size());
	std::copy(source.begin(), source.end(), dest.begin());

}

void convertToStr(std::string &dest, TCHAR * source)
{
	std::wstring buff = source;
	dest.resize(buff.size());
	WideCharToMultiByte(CP_ACP, 0, &buff[0], (int)buff.size(), &dest[0], dest.size(), 0, 0);
}

class ppp
	:public IModule
{
public:
	ppp();
	~ppp();
	virtual void pull(memory::BulletChain *chain) override;
	virtual void push(memory::ParamChain chain) override;

private:
	xml::XMLNode * root;
};


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ExpWnd exp;
	ppp ps;
	exp.init(&ps);

	return autownd::msgLoop();

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return main();
}

ppp::ppp()
{
	root = nullptr;
	xml::XMLParser ps;
	ps.parseFile("iecu.xml");
	root = ps.pickupDocument();
}

ppp::~ppp()
{
	delete root;
}

void ppp::pull(memory::BulletChain * chain)
{
	const char *cat = chain->first()->pdata<char>();
	LPARAM param = *chain->at()->data<LPARAM>();

	if (memory::streql(cat, "read"))
	{
		const xml::XMLNode::AttNode *att = nullptr;
		xml::XMLNode *node = (xml::XMLNode*)param;
		while (att = node->getAttribute(att))
		{
			std::wstring buff; convertToWStr(buff, att->getKey());
			chain->add()->fill(buff.c_str(), buff.size()+1);
			convertToWStr(buff, att->getValue());
			chain->add()->fill(buff.c_str(), buff.size()+1);
		}
	}
	else
	{
		xml::XMLNode *node;
		if (param == 0) node = root;
		else node = (xml::XMLNode*)param;

		if (node->getType() != xml::ELEMENT_NODE) {
			chain->at()->fill(1);
			return;
		}

		wstring buff; convertToWStr(buff, node->getString());
		chain->add()->fill(buff.c_str(), buff.size() + 1);
		chain->add()->fill((int)node->getType());
		chain->add()->fill((LPARAM)node->getParent());

		node = node->getFirstChild();
		while (node)
		{
			convertToWStr(buff, node->getString());
			chain->add()->fill(buff.c_str(), buff.size() + 1);
			chain->add()->fill((int)node->getType());
			chain->add()->fill((LPARAM)node);
			node = node->getNext();
		}
	}
}

void ppp::push(memory::ParamChain chain)
{
	if (memory::streql(chain.begin()->first, "setkey")) {
		std::string oldkey, keystr, valuestr;
		xml::XMLNode *node;
		convertToStr(oldkey, (chain.begin())->second.pdata<TCHAR>());
		convertToStr(keystr, (chain.begin() + 1)->second.pdata<TCHAR>());
		convertToStr(valuestr, (chain.begin() + 2)->second.pdata<TCHAR>());
		node = (xml::XMLNode*)*((chain.begin() + 3)->second.data<long>());
		node->updateAttribute(oldkey, keystr, valuestr);
		return;
	}
	if (memory::streql(chain.begin()->first, "setstr")) {
		std::string str;
		xml::XMLNode *node;
		convertToStr(str, (chain.begin())->second.pdata<TCHAR>());
		node = (xml::XMLNode*)*((chain.begin() + 1)->second.data<long>());
		node->setString(str);
	}
}
