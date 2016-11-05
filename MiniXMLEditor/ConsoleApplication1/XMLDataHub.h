#pragma once
class XMLDataHub
	:public IDataHub
{
public:
	XMLDataHub();
	~XMLDataHub();

	int getChildItemData(LPARAM param, ItemPool *pool) override;
	int getItemAtt(LPARAM param, AttPool *pool) override;
	LPARAM queryParent(LPARAM param) override;

	void loadFile(const char * name);

	int queryItem(ItemData *pool) override;
	int setItem(ItemData *source) override;

	int queryPath(LPARAM param, std::wstring &path) override;

	int setItemAtt(LPARAM param, std::wstring oldkey, std::wstring value, std::wstring nekey) override;


	int addItem(LPARAM param, std::string text, DOMPos pos, ItemPool *record) override;

private:
	xml::XMLNode *theNode;
};

