#pragma once

#include <vector>
using namespace std;

typedef int index;

#define NUMBER_OF_CONECTIONS	2

class layoutNumericConection
{
public:
	layoutNumericConection(index myIndex);
	bool initialConection(layoutNumericConection& newConection); //with marks changed accordingly
	bool isMarked();

	void mark();
	void unmark();

	void secondaryConection(layoutNumericConection& newConection); //doesn't change marks
private:
	index myself;
	bool marked;
	vector<index> conections;

	void initialCheckMark();
	bool isAlreadyConectedToMe(layoutNumericConection& newConection);
	void forcedConection(layoutNumericConection& newConection); //doens't change marks 
};

