#pragma once
#include <malloc.h>
#include <typeinfo>
#include <utility>

namespace memory
{
	//tiny class for memeory control
	class Block
	{
	public:
		Block() :theBlock(nullptr) {}
		Block(const Block & other) {
			clone(other);
		}
		Block(Block && other) {
			swap(other);
		}
		~Block() {
			if (theBlock) delete theBlock;
		}
		inline void clone(const Block &other) {
			theBlock = new char[other.size()];
			memcpy(theBlock, other.theBlock, other.size());
		}
		inline void swap(Block &other) {
			void * temp = other.theBlock;
			other.theBlock = theBlock;
			theBlock = temp;
		}
		inline size_t size() const {
			return _msize(theBlock);
		}
		void resize(size_t size) {
			if (theBlock) delete theBlock;
			theBlock = new char[size];
		}
		inline void * address() { return theBlock; }
		inline const void * address() const { return theBlock; }

	private:
		void * theBlock;
	};

	//bullet
	class Bullet
	{
	public:
		//for saving address
		template<class T> Bullet(T* source) {
			fill(&source, 1);
		}
		//for saving data
		template<class T> Bullet(const T& source) {
			fill(&source, 1);
		}
		//for saving a batch of data
		template<class T> Bullet(T* source, size_t size) {
			fill(source, size);
		}

		Bullet();
		Bullet(const Bullet & other);
		Bullet(Bullet && other);
		~Bullet() {};

		//clone
		void clone(const Bullet & other);
		void swap(Bullet & other);

		//save single data
		template<class T> inline void fill(const T& source) {
			fill(&source, 1);
		}
		template<class T> inline void fill(T* source) {
			fill(&source, 1);
		}

		//save to this
		template<class T> inline void fill(T* source, size_t size) {
			static const char * type = typeid(T).name();
			theValue.resize(size * sizeof(T));
			memcpy(theValue.address(), source, size * sizeof(T));
			theType = type;
		}

		//get the data
		template<class T> inline void inject(T* dest, size_t maxCount) const {
			static const char * type = typeid(T).name();
			if (theType == type) memcpy(dest, theValue.address(), theValue.size() > maxCount * sizeof(T) ? maxCount * sizeof(T) : theValue.size());
		}

		//get the data address
		inline void* operator*() {
			return theValue.address();
		}

		inline const void* operator*() const {
			return theValue.address();
		}

		//get the size
		inline size_t size() const { return theValue.size(); }

		//use it directly
		template<class T> T* data() const {
			return (T*)operator*();
		}

		template<class T> T* pdata() const {
			return *(T**)operator*();
		}

	private:
		Block theValue;
		const char * theType;
	};

	//bulletchain
	class BulletChain
	{
	public:
		BulletChain(size_t size);
		~BulletChain();

		Bullet * first();
		Bullet * add();
		void addLine();
		Bullet * at();
		int line();
	private:
		struct Connect {
			Bullet bullet;
			Connect * next;
			size_t id;
			Connect() :next(nullptr),id(0) {}
			~Connect() { if (next) delete next; }
		} theFirst, *theLast, *theCurrent;
		size_t theSize;
	};

	//ParamSet
	typedef std::pair<const char*, memory::Bullet> Param;
	typedef std::initializer_list<Param> ParamChain;

	//str compare function
	inline bool streql(const char* left, const char * right) {
		while (*left == *right) {
			if (*left == '\0') return true;
			left++; right++;
		}
		return false;
	}

	//find value
	template<class T>
	int find(const ParamChain params, const char* key, T& dest) {
		for (ParamChain::iterator it = params.begin(); it != params.end(); it++) {
			if (streql(it->first, key)) {
				it->second.inject(&dest, 1);
				return 1;
			}
		}
		return 0;
	}
}
