#pragma once
#include "XMLNode.h"
#include <list>

namespace xml
{
	namespace condition
	{
		class IXMLCondition
		{
		public:
			virtual ~IXMLCondition() = default;
			virtual std::string getString() = 0;

			virtual int isMatch(XMLNode * node) = 0;
		};

		class XMLLogic
			:public IXMLCondition
		{
		public:
			XMLLogic() = default;
			~XMLLogic();

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
			:public XMLLogic
		{
		public:
			XMLAnd() = default;
			~XMLAnd() = default;

			std::string getString() override { return "AND"; }

			int isMatch(XMLNode *node) override;

		private:
		};

		class XMLOr
			:public XMLLogic
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

			std::string getString() override { return "=="; }

			int isMatch(XMLNode *node) override;

			void setCondition(std::string str, NodeType type);

		private:
			NodeType theType;
			std::string theStr;
		};

		class XMLChildContain
			:public IXMLCondition
		{
		public:
			XMLChildContain();
			~XMLChildContain() = default;

			std::string getString() override { return "CONTAIN"; }

			int isMatch(XMLNode *node) override;

			void setChildConditon(IXMLCondition * con) { theChildCondition = con; }

		private:
			IXMLCondition *theChildCondition;
		};
	}
}


