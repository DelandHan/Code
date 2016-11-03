#pragma once
class XMLDataHub
	:public IDataHub
{
public:
	XMLDataHub();
	~XMLDataHub();

	int getItemData(LPARAM parent, ItemPool *pool) override;

	LPARAM queryParent(LPARAM param) override;

	void loadFile(const char * name);

	int setItem(ItemData *source) override;

	int queryPath(LPARAM param, std::wstring &path) override;

	int setItemAtt(LPARAM param, std::wstring oldkey, std::wstring value, std::wstring nekey) override;

	int addItem(LPARAM position, std::string text, AddAction action) override;

private:
	xml::XMLNode *theNode;
};

