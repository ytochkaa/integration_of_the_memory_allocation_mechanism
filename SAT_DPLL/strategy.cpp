#include "strategy.h"  
#include "boolequation.h"

#include <algorithm>
#include <vector>

int IStrategy::ChooseColumn(BoolEquation &equation) const {
    std::vector<int> indexes;
	std::vector<int> values;
	bool rezInit = false;

	for (int i = 0; i < equation.mask.getSize(); i++) {
		if (equation.mask[i] == 0) {
			indexes.push_back(i);
		}
	}

	for (int i = 0; i < equation.cnfSize; i++) {
		BoolInterval *interval = equation.cnf[i];

		if (interval != nullptr) {
			if (!rezInit) {
				for (int k = 0; k < indexes.size(); k++) {
					if (interval->getValue(indexes.at(k)) == '-') {
						values.push_back(1);
					} else {
						values.push_back(0);
					}
				}

				rezInit = true;
			} else {
				for (int k = 0; k < indexes.size(); k++) {
					if (interval->getValue(indexes.at(k)) == '-') {
						//int val = values.at(k) + (interval->getValue(indexes.at(k)) - '0');
						values.at(k)++;
					}
				}
			}
		}
	}

	int minElementIndex = std::min_element(values.begin(), values.end()) - values.begin();

	return indexes.at(minElementIndex);
}