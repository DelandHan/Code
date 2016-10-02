#pragma once

class IModule
{
public:
	virtual void pull(memory::BulletChain *chain) = 0;
	virtual int push(memory::ParamChain chain) = 0;
};

