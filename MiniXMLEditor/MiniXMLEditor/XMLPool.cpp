#include "stdafx.h"
#include "XMLPool.h"
#include <fstream>

using namespace std;
using namespace xml;

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

XMLPool::XMLPool()
{
	theRoot = nullptr;
}


XMLPool::~XMLPool()
{
	delete theRoot;
}

int XMLPool::pull(memory::BulletChain * chain)
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
			chain->add()->fill(buff.c_str(), buff.size() + 1);
			convertToWStr(buff, att->getValue());
			chain->add()->fill(buff.c_str(), buff.size() + 1);
		}
	}

	if (memory::streql(cat, "item"))
	{
		xml::XMLNode *node = (xml::XMLNode*)param;	
		std::wstring buff; convertToWStr(buff, node->getString());

		chain->add()->fill(buff.c_str(), buff.size() + 1);
	}

	if (memory::streql(cat, "childs"))
	{
		xml::XMLNode *node;
		if (param == 0) node = theRoot;
		else node = (xml::XMLNode*)param;

		if (node == nullptr) return 2;

		if (node->getType() != xml::ELEMENT_NODE && node->getType() != xml::DOCUMENT_NODE) {
			return 1;
		}

		//get path
		xml::XMLNode *child_n = node; size_t size = 0; wstring buff; string buffA;
		while (child_n) {
			size += child_n->getString().size() + 1;
			child_n = child_n->getParent();
		}
		buffA.resize(size + 1);

		child_n = node;
		while (child_n) {
			memcpy(&buffA[size - child_n->getString().size()], child_n->getString().c_str(), child_n->getString().size());
			buffA[size - child_n->getString().size() - 1] = '\\';
			size -= child_n->getString().size() + 1;
			child_n = child_n->getParent(); 
		}

		convertToWStr(buff, buffA);
		chain->add()->fill(buff.c_str(), buff.size() + 1);
		chain->add()->fill((int)node->getType());
		chain->add()->fill((LPARAM)node->getParent());

		//get child items
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

	return 0;
}

int XMLPool::push(memory::ParamChain chain)
{
	if (memory::streql(chain.begin()->first, "setkey")) {
		std::string oldkey, keystr, valuestr;
		xml::XMLNode *node;
		convertToStr(oldkey, (chain.begin())->second.pdata<TCHAR>());
		convertToStr(keystr, (chain.begin() + 1)->second.pdata<TCHAR>());
		convertToStr(valuestr, (chain.begin() + 2)->second.pdata<TCHAR>());
		node = (xml::XMLNode*)*((chain.begin() + 3)->second.data<long>());
		if (oldkey.size()) //if we already have this key,
		{
			if (keystr.size() == 0) node->removeAttribute(oldkey); //if it did not set a new key str, remove the key
			else node->setAttribute(oldkey, valuestr, keystr); // update the key

		}
		else
		{
			node->setAttribute(keystr, valuestr); //add a new key.
		}
		return 0;
	}
	if (memory::streql(chain.begin()->first, "setstr")) {
		std::string str;
		xml::XMLNode *node;
		convertToStr(str, (chain.begin())->second.pdata<TCHAR>());
		node = (xml::XMLNode*)*((chain.begin() + 1)->second.data<long>());
		node->setString(str);
		return 0;
	}
	if (memory::streql(chain.begin()->first, "del")) {
		xml::XMLNode *node = (xml::XMLNode*)*((chain.begin())->second.data<long>());
		if(node) delete node;
		return 0;
	}
	if (memory::streql(chain.begin()->first, "ins_a")) {
		xml::XMLNode *node = (xml::XMLNode*)*((chain.begin())->second.data<long>()),
			*neNode = new XMLNode;
		node->insert(neNode, true);	
		neNode->setString("NewNode");
		return 0;
	}
	if (memory::streql(chain.begin()->first, "ins_b")) {
		xml::XMLNode *node = (xml::XMLNode*)*((chain.begin())->second.data<long>()),
			*neNode = new XMLNode;
		neNode->setString("NewNode");
		node->insert(neNode, false);
		return 0;
	}

	if (memory::streql(chain.begin()->first, "chgtyp")) {
		xml::XMLNode *node = (xml::XMLNode*)*((chain.begin())->second.data<long>());
		if(node->getType()==xml::ELEMENT_NODE) node->convertType(xml::TEXT_NODE);
		else node->convertType(xml::ELEMENT_NODE);
		return 0;
	}

	if (memory::streql(chain.begin()->first, "append")) {
		xml::XMLNode *node = (xml::XMLNode*)*((chain.begin())->second.data<long>()),
			*neNode = new XMLNode;
		neNode->setString("NewNode");
		if(node) node->append(neNode);
		else {
			theRoot = new XMLNode;
			theRoot->convertType(xml::DOCUMENT_NODE);
			theRoot->append(neNode);
		}
		return 0;
	}

	if (memory::streql(chain.begin()->first, "open")) {
		xml::XMLParser ps;
		string str;
		convertToStr(str, chain.begin()->second.pdata<TCHAR>());
		ps.parseFile(str.c_str());
		if (theRoot) delete theRoot;
		theRoot = ps.pickupDocument();
		return 0;
	}

	if (memory::streql(chain.begin()->first, "save")) {
		xml::XMLParser ps;
		string str;
		convertToStr(str, chain.begin()->second.pdata<TCHAR>());
		if (str.size() <= 4) return 1;
		if (memcmp(&str[str.size() - 4], ".xml", 4) != 0) str += ".xml";
		ofstream of(str);
		ps.saveNode(theRoot, &of);
		of.close();
		return 0;
	}

	return 1;
}
