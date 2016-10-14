#pragma once
#include "IModule.h"

class XMLPool
	:public IModule
{
public:
	XMLPool();
	~XMLPool();

	virtual int pull(memory::BulletChain *chain) override;
	virtual int push(memory::ParamChain chain) override;

private:
	xml::XMLNode *theRoot;

};

