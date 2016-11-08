#pragma once

#include <vector>

namespace parser
{
	class Chain
	{
	public:
		Chain();
		Chain(const char * start, size_t count);
		Chain(const Chain &other);
		~Chain();

		void assign(const char * source, size_t count);

		void cut(const char *c);//cut from c

		inline const char * pop() { return thePos > theEnd ? nullptr : thePos++; }
		inline const char * get() const { return thePos > theEnd ? nullptr : thePos; }
		inline const char * getStart() const { return theStart; }
		inline const char * getEnd() const { return theEnd; }
		inline void move(size_t count) { thePos += count; }

		inline int suffix(const char *pos) const { return theEnd - pos; }
		inline int prefix(const char *pos) const { return pos - theStart; }

	private:
		const char * theStart;
		const char * thePos;
		const char * theEnd;

	};
	
	class ChainChecker
	{
	public:
		ChainChecker();
		~ChainChecker();

		int check(Chain * inchain, Chain * subchain);

		int addEndStr(std::string end_str);

	private:
		inline size_t firstcheck(char c) { return theIndex.find(c); }
		int match(Chain * oncheck, size_t index);

		std::string theIndex;
		
		typedef std::vector<std::string> Tokens;
		std::vector<Tokens> theTokenIndex;
	};
}