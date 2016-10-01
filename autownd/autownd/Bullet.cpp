#include "Bullet.h"

using namespace memory;

memory::Bullet::Bullet()
	:theType(nullptr)
{
}

Bullet::Bullet(const Bullet & other)
{
	theValue.clone(other.theValue);
	theType = other.theType;
}

Bullet::Bullet(Bullet && other)
{
	theValue.swap(other.theValue);
	const char * temp = theType;
	theType = other.theType;
	other.theType = temp;
}

void Bullet::clone(const Bullet & other)
{
	theType = other.theType;
	theValue.clone(other.theValue);
}

void Bullet::swap(Bullet & other)
{
	const char * temp = theType;
	theType = other.theType;
	other.theType = temp;
	theValue.swap(other.theValue);
}

///////////////////////

memory::BulletChain::BulletChain(size_t size)
	:theLast(&theFirst), theCurrent(&theFirst), theSize(size)
{
	if (theSize == 0) theSize = 1;
	addLine();
}

memory::BulletChain::~BulletChain()
{

}

Bullet * memory::BulletChain::first()
{
	theCurrent = theFirst.next;
	return &theFirst.bullet;
}

Bullet * memory::BulletChain::add()
{
	Connect *temp = new Connect;
	theLast->next = temp;
	if (theLast->id < theSize - 1) temp->id = theLast->id + 1;
	else temp->id = 0;
	theLast = temp;
	return &temp->bullet;
}

void memory::BulletChain::addLine()
{
	while (theLast->id < theSize - 1)
	{
		add();
	}
}

Bullet * memory::BulletChain::at()
{
	if (theCurrent->next == nullptr || theCurrent->next->id == 0) return &theCurrent->bullet;
	
	Bullet * bullet = &theCurrent->bullet;
	theCurrent = theCurrent->next;
	return bullet;
}

int memory::BulletChain::line()
{
	while (theCurrent->id && theCurrent->next) theCurrent = theCurrent->next;
	if (theCurrent->next == nullptr) return 0;
	else return 1;
}



