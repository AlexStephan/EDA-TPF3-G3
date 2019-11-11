#include "layoutNumericConection.h"

layoutNumericConection::layoutNumericConection(index myIndex)
	: myself(myIndex), marked(false), conections()
{
}

bool layoutNumericConection::initialConection(layoutNumericConection& newConection)
{
	if ((isAlreadyConectedToMe(newConection) == false) &&
		(myself != newConection.myself)) {

		forcedConection(newConection);
		initialCheckMark();
		newConection.forcedConection(*this);
		newConection.initialCheckMark();
	}

	return marked;
}

bool layoutNumericConection::isMarked()
{
	return marked;
}

void layoutNumericConection::mark()
{
	marked = true;
}

void layoutNumericConection::unmark()
{
	marked = false;
}

void layoutNumericConection::secondaryConection(layoutNumericConection& newConection)
{
	if ((isAlreadyConectedToMe(newConection) == false) &&
		(myself != newConection.myself)) {

		initialCheckMark();
		newConection.initialCheckMark();
	}
}

void layoutNumericConection::forcedConection(layoutNumericConection& newConection)
{
	conections.emplace_back(newConection.myself);
}

void layoutNumericConection::initialCheckMark()
{
	if (conections.size() >= NUMBER_OF_CONECTIONS)
		marked = true;
}

bool layoutNumericConection::isAlreadyConectedToMe(layoutNumericConection& newConection)
{
	bool answer = false;
	for (int i = 0;
		(answer == false) && (i < conections.size());
		i++) {

		if (conections[i] == newConection.myself)
			answer = true;
	}

	return answer;
}
