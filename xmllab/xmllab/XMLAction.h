#pragma once
#include "XMLNode.h"

namespace xml
{
	namespace action
	{
		class IXMLAction
		{
		public:
			virtual ~IXMLAction() = default;
			virtual xml::XMLNode * handle(xml::XMLNode * node) = 0;
		};

		class XMLDel
			:public IXMLAction
		{
		public:
			xml::XMLNode * handle(xml::XMLNode * node) override;

		};

		class XMLRemove
			:public IXMLAction
		{
		public:
			xml::XMLNode * handle(xml::XMLNode * node) override;
		};

		class XMLReplace
			:public IXMLAction
		{
		public:
			XMLReplace();
			~XMLReplace() = default;
			xml::XMLNode * handle(xml::XMLNode * node) override;

			void setNewStatus(std::string str, NodeType type);

		private:
			std::string theStr;
			NodeType theType;
		};
	}
}
