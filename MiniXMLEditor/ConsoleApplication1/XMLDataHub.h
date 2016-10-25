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

	int insertAfter(LPARAM param, std::string text) override;
	int append(LPARAM parent, std::string text) override;
	int insertBefore(LPARAM param) override;

private:
	xml::XMLNode *theNode;
};

