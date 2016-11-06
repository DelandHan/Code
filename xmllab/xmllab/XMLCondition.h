#pragma once
#include "XMLNode.h"
#include <list>

namespace xml
{
	class IXMLCondition
	{
	public:
		virtual ~IXMLCondition() = default;

		virtual int isMatch(XMLNode * node) = 0;
	};

	class XMLConditionContainer
		:public IXMLCondition
	{
	public:
		XMLConditionContainer() = default;
		~XMLConditionContainer();

		virtual std::string getString() = 0;

		template<class T>
		typename std::enable_if<std::is_base_of<xml::IXMLCondition, T>::value, T>::type * addCondition() {
			T * temp = new T;
			theChilds.push_back(temp);
			return temp;
		}

		void addCondition(IXMLCondition * con);

	protected:
		std::list<IXMLCondition*> theChilds;
	};

	class XMLAnd
		:public XMLConditionContainer
	{
	public:
		XMLAnd() = default;
		~XMLAnd() = default;

		std::string getString() override { return "AND"; }

		int isMatch(XMLNode *node) override;

	private:
	};

	class XMLOr
		:public XMLConditionContainer
	{
	public:
		XMLOr() = default;
		~XMLOr() = default;

		std::string getString() override { return "OR"; }

		int isMatch(XMLNode *node) override;

	private:
	};
	
	class XMLEqual
		:public IXMLCondition
	{
	public:
		XMLEqual();
		~XMLEqual() = default;

		int isMatch(XMLNode *node) override;

		void setCondition(std::string str, NodeType type);

	private:
		NodeType theType;
		std::string theStr;
	};
}


