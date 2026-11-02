#include "strategy.h"
#include "boolequation.h"

// Выбирает свободный столбец с минимальным числом символов '-'.
int MostConstrainedStrategy::ChooseColumn(BoolEquation &equation) const {
	int bestColumn = -1;
	int bestCount = 0;

	for (int col = 0; col < equation.mask.getSize(); col++) {
		// Единица в маске: столбец уже обработан, ветвиться по нему нельзя.
		if (equation.mask[col] != 0)
			continue;

		// Считаем '-' только в ещё не выполненных дизъюнкциях (nullptr - уже выполнена).
		int dontCares = 0;
		for (int i = 0; i < equation.cnfSize; i++) {
			if (equation.cnf[i] != nullptr && equation.cnf[i]->getValue(col) == '-')
				dontCares++;
		}

		// При равенстве остаётся столбец с меньшим индексом.
		if (bestColumn < 0 || dontCares < bestCount) {
			bestColumn = col;
			bestCount = dontCares;
		}
	}

	return bestColumn;
}

// Выбирает первый свободный столбец.
int FirstFreeColumnStrategy::ChooseColumn(BoolEquation &equation) const {
	for (int i = 0; i < equation.mask.getSize(); i++) {
		if (equation.mask[i] == 0)
			return i;
	}
	return -1;
}
