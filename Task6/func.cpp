#include "pch.h"
#include "func.h"

std::vector<std::vector<double>> PotentialInNextTime(std::vector<std::vector<double>> potentialInTime, std::vector<std::vector<double>> potentialStart)
{
	std::vector<std::vector<double>> out(potentialInTime.size(), std::vector<double>(potentialInTime.size()));
	//усредняем значения в узлах соседних
	//шаг = 1
	for (int i = 0; i < potentialInTime.size(); i++) {
		for (int j = 0; j < potentialInTime.size(); j++) {
			//граничные условия
			if (i == 0 || j == 0 || i == potentialInTime.size() - 1 || j == potentialInTime.size() - 1) {
				out[i][j] = 0;
			}
			else if (potentialStart[i][j] != 0)//попускаем металл
			{
				out[i][j] = potentialStart[i][j];
			}
			else {
				double summa = 0;
				int count = 0;
				if (i > 0) {
					summa += potentialInTime[i - 1][j];
					count++;
				}
				if (j > 0) {
					summa += potentialInTime[i][j - 1];
					count++;
				}
				if (i < potentialInTime.size() - 1) {
					summa += potentialInTime[i + 1][j];
					count++;
				}
				if (j < potentialInTime.size() - 1) {
					summa += potentialInTime[i][j + 1];
					count++;
				}
				out[i][j] = summa / count;
			}
		}
	}
	return out;
}
