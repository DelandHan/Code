#include "XMLNode.h"

using namespace std;
using namespace xml;

///////////////////////AttNode///////////////////////
XMLNode::AttNode::AttNode()
	:theNext(nullptr), thePrev(nullptr)
{
}

XMLNode::AttNode::~AttNode()
{
	if (theNext)
		theNext->thePrev = thePrev;
	if (thePrev)
		thePrev->theNext = theNext;
}

void XMLNode::AttNode::clear()
{
	if (theNext) {
		theNext->clear();
	}
	delete this;
}

XMLNode::AttNode * xml::XMLNode::AttNode::insert(const std::string & key, const std::string & value)
{
	AttNode * temp = new AttNode;
	temp->theKey.assign(key.c_str(), XMLNode::verifyString(key.c_str(), key.size(), ELEMENT_NODE)); temp->theValue = value;
	temp->thePrev = this;
	if (this)
	{
		temp->theNext = theNext;
		theNext = temp;
	}
	return temp;
}

///////////////////////XMLNode///////////////////////

XMLNode::XMLNode(NodeType type) :
	thePrevious(nullptr), theNext(nullptr),
	theParent(nullptr), theFirstChild(nullptr), theLastChild(nullptr),
	theAtt(nullptr), theType(type)
{
}

XMLNode::~XMLNode()
{
	if (theAtt) {
		theAtt->clear();
	}

	//tree
	XMLNode* node = theFirstChild, *temp;

	while (node) {
		temp = node; node = node->getNext();
		delete temp;
	}

	removeMe();
}

XMLNode * xml::XMLNode::clone()
{
	XMLNode *node = new XMLNode, *temp = this->theFirstChild;
	*node = *this;
	while (temp) {
		node->append(temp->clone());
		temp = temp->theNext;
	}
	return node;
}

XMLNode * xml::XMLNode::getNextByChain(bool checkChild) const
{
	XMLNode * pNode = nullptr;
	if (checkChild) pNode = this->getFirstChild();
	if (pNode == nullptr) {
		pNode = const_cast<XMLNode *>(this); //for const usage
		while (pNode->getNext() == nullptr) {
			if (pNode->getParent() == nullptr) return nullptr;
			else pNode = pNode->getParent();
		}
		pNode = pNode->getNext();
	}
	return pNode;
}

XMLNode * xml::XMLNode::getPreviousByChain(bool checkChild)
{
	XMLNode * pNode = this->getPrevious();
	if (pNode == nullptr) return this->getParent();
	if (pNode->getLastChild() && checkChild) return pNode->getLastChild();
	return pNode;
}

void xml::XMLNode::insert(XMLNode * neighbour, bool isNext)
{
	//if it is not individual, return
	if (neighbour->theNext || neighbour->thePrevious || neighbour->theParent) return;
	if (isNext)
	{//insert the node to next.
	 //configure neighbour first.
		neighbour->thePrevious = this;
		neighbour->theNext = theNext;

		if (theNext != nullptr)
		{
			theNext->thePrevious = neighbour;
		}
		else
		{
			if (theParent)
			{
				if (theParent->theLastChild == this)
				{
					theParent->theLastChild = neighbour;
				}
			}
		}
		theNext = neighbour;
	}
	else
	{//insert the node to previous
	 //configure neighbour first
		neighbour->theNext = this;
		neighbour->thePrevious = thePrevious;
		if (thePrevious != nullptr)
		{
			thePrevious->theNext = neighbour;
		}
		else
		{
			if (theParent)
			{
				if (theParent->theFirstChild == this)
				{
					theParent->theFirstChild = neighbour;
				}
			}
		}
		thePrevious = neighbour;
	}
	neighbour->theParent = this->theParent;
}

void xml::XMLNode::append(XMLNode * child)
{
	if (child->theParent || child->thePrevious || child->theNext) return;
	if (theFirstChild == nullptr && theLastChild == nullptr)
	{
		theFirstChild = child; theLastChild = child;
		child->theParent = this;
		return;
	}
	else
	{
		theLastChild->insert(child, true);
	}
}

void xml::XMLNode::removeMe()
{
	if (theParent)
	{
		if (theParent->theFirstChild == this) 	theParent->theFirstChild = this->theNext;
		if (theParent->theLastChild == this) theParent->theLastChild = this->thePrevious;
	}

	if (thePrevious != nullptr) thePrevious->theNext = theNext;
	if (theNext != nullptr) theNext->thePrevious = thePrevious;

	thePrevious = nullptr;
	theNext = nullptr;
	theParent = nullptr;
}

XMLNode & xml::XMLNode::operator=(const XMLNode & other)
{
	theString = other.theString;
	theType = other.theType;
	if (other.theAtt)
	{
		if (theAtt) {
			theAtt->clear();
			theAtt = nullptr;
		}
		AttNode * node = other.theAtt;
		while (node)
		{
			setAttribute(node->theKey, node->theValue);
			node = node->theNext;
		}
	}
	return *this;
}

void xml::XMLNode::convertType(NodeType type)
{
	if (theType == ELEMENT_NODE && type != ELEMENT_NODE)
	{
		if (theAtt)
		{
			theAtt->clear();
			theAtt = nullptr;
		}
		XMLNode* node = getFirstChild();
		while (node)
		{
			XMLNode* temp = node;
			node = node->getNext();
			delete temp;
		}
	}
	theString.resize(verifyString(theString.c_str(), theString.size(), type));
	theType = type;
}

void xml::XMLNode::setString(const char * source, size_t count, size_t off)
{
	if (off == null)
	{
		theString.assign(source, verifyString(source, count));
	}
	else
	{
		theString.insert(off, source, verifyString(source, count));
	}
}

std::string xml::XMLNode::getAttribute(const std::string & key) const
{
	AttNode * node = theAtt;
	while (node)
	{
		if (node->theKey == key)
			return node->theValue;
		else
			node = node->theNext;
	}
	return "";
}

const XMLNode::AttNode * xml::XMLNode::getAttribute(const AttNode * node) const
{
	if (node)
	{
		return node->theNext;
	}
	else
	{
		return theAtt;
	}
}

void xml::XMLNode::setAttribute(const std::string &key, const std::string &value, const std::string &neKey)
{
	if (key.size() == 0) return;
	if (theType == ELEMENT_NODE) {
		AttNode *node = theAtt, *prev = nullptr;
		while (node) {
			if (node->theKey == key) {
				if (neKey.size() != 0) node->theKey.assign(neKey.c_str(), verifyString(neKey.c_str(), neKey.size(), ELEMENT_NODE));//verify att key name as element node name.
				node->theValue = value;
				return;
			}
			prev = node;
			node = node->theNext;
		}
		prev = prev->insert(key, value);
		if (theAtt == nullptr) theAtt = prev;
	}

}

void xml::XMLNode::removeAttribute(const std::string & key)
{
	AttNode * node = theAtt;
	while (node)
	{
		if (node->theKey == key)
		{
			if (node == theAtt) theAtt = node->theNext;
			delete node;
			return;
		}
		node = node->theNext;
	}
}

size_t XMLNode::verifyString(const char * source, size_t count)
{
	return verifyString(source, count, theType);
}

size_t XMLNode::verifyString(const char * source, size_t count, NodeType type)
{
	if (type == ELEMENT_NODE)
	{
		if (!isalpha(*source)) return 0;
		for (const char * c = source + 1; c < source + count; c++)
		{
			if (*c <= 32 || *c == '&' || *c == ';') return c - source;

		}
	}

	return count;
}


/*


std::pair<std::string, std::string> XMLNode::getAttribute(const std::string & name, bool goNext)
{
if (m_Attributes) {
XMLAttributeSet::iterator it;
if (name.size() == 0) return *(m_Attributes->begin());
it = m_Attributes->find(name);
if (it == m_Attributes->end()) return std::pair<std::string, std::string>("", ""); //not found
if (goNext) it++;
if (it == m_Attributes->end()) {
return std::pair<std::string, std::string>("", "");
}
else {
return *it;
}
}
else
return std::pair<std::string, std::string>("", "");
}



}
*/