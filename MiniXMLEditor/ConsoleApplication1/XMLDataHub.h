#pragma once
class XMLDataHub
	:public IDataHub
{
public:
	XMLDataHub();
	~XMLDataHub();

	virtual int getChildItemData(LPARAM param, ItemPool *pool) override;
	virtual int getItemAtt(LPARAM param, AttPool *pool) override;
	virtual LPARAM queryParent(LPARAM param) override;

	void loadFile(const char * name);

private:
	xml::XMLNode *theNode;
};

