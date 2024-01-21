#include "pch.h"
#include "DynamicProgramm.h";

//--------------------------------------------------------------------------------||

std::string DynamicProgramm::function = "";
int DynamicProgramm::MaxResource = 0;
int DynamicProgramm::CountProduct = 0;
int DynamicProgramm::nmax = 0;
double DynamicProgramm::bmax = 0;

std::vector<int> DynamicProgramm::Limitations;

std::vector <double> DynamicProgramm::Kj;
std::vector <double> DynamicProgramm::Aj;

std::vector<std::vector<int>> DynamicProgramm::xj;
std::vector<std::vector<int>> DynamicProgramm::noc;
std::vector<std::vector<double>> DynamicProgramm::bellmanFunction;

//--------------------------------------------------------------------------------||

void DynamicProgramm::Calc(const std::string file) {

	ReadInput(file);

	CountProduct = Limitations.size();

	for (int i = 0; i <= MaxResource; i++) {
		bellmanFunction.push_back(std::vector<double>());
		noc.push_back(std::vector <int>());
		for (int j = 0; j < CountProduct; j++) {
			noc[i].push_back(0);
			bellmanFunction[i].push_back(0);
		}
	}

	FuncBellman();

	PrintBellman();


	std::vector <std::vector<int>> ksi;
	for (int i = 0; i <= MaxResource; i++) {
		ksi.push_back(std::vector<int>());
		xj.push_back(std::vector<int>());
		for (int j = 0; j < CountProduct; j++) {
			ksi[i].push_back(0);
			xj[i].push_back(0);
		}
	}

	for (int i = 0; i <= MaxResource; i++) {
		xj[i][CountProduct - 1] = noc[i][CountProduct - 1];
		ksi[i][CountProduct - 1] = i+1;
		for (int j = CountProduct - 1; j >= 1; j--) {
			ksi[i][j - 1] = ksi[i][j] - Limitations[j] * xj[i][j];
			xj[i][j - 1] = noc[ksi[i][j - 1] - 1][j - 1];
		}
	}
	
	std::string optX = "[";
	for (auto num : xj[xj.size() - 1]) {
		optX += std::to_string(num) + "], [";
	}
	optX = optX.substr(0, optX.size() - 3);
	std::cout << "\n Оптимальное решение:\n " + optX + "\n Оптимальное значение целевой функции: W = " << bellmanFunction[bellmanFunction.size() - 1][CountProduct - 1];

}

//--------------------------------------------------------------------------------||

void DynamicProgramm::FuncBellman() {
	int lh = 0;
	std::vector<double> wr;
	for (int i = 0; i <= MaxResource; i++)
		wr.push_back(0);

	for (int i = 1; i <= MaxResource + 1; i++) {
		lh = (i - 1) / Limitations[0] + 1;
		if (lh != 0) {
			for (int j = 0; j < lh; j++) {
				wr[j] = Func(0, j);
			}
			getMax(wr, lh);
			bellmanFunction[i - 1][0] = bmax;
			noc[i - 1][0] = nmax - 1;
		}
	}

	for (int nper = 1; nper < CountProduct; nper++) {
		for (int i = 0; i <= MaxResource; i++) {
			lh = i / Limitations[nper] + 1;
			for (int j = 0; j < lh; j++) {
				int tmp = i + 1 - (j + 1) * Limitations[nper] + Limitations[nper];
				wr[j] = Func(nper, j) + bellmanFunction[tmp - 1][nper - 1];
			}
			getMax(wr, lh);
			bellmanFunction[i][nper] = bmax;
			noc[i][nper] = nmax - 1;
		}
	}
}

double DynamicProgramm::Func(int nper, int res) {
	return Aj[nper] * (1 - pow(1 - Kj[nper], res));
}

void DynamicProgramm::getMax(std::vector<double> wr, int lh) {
	nmax = 1;
	bmax = wr[0];
	if (lh == 1) return;
	for (int i = 1; i < lh; i++) {
		if (wr[i] > bmax) {
			nmax = i + 1;
			bmax = wr[i];
		}
	}
}

//--------------------------------------------------------------------------------||

void DynamicProgramm::ReadInput(const std::string path) {
	std::ifstream ifile(path);

	if (ifile.is_open()) {
		std::string buff;

		std::cout << "\n Считывание входного файла...";

		while (ifile.eof(), ifile >> buff) {

			// Считывание ограничений
			if (buff == "\"a\":") {
				ifile >> buff;
				buff = buff.substr(1, buff.size());

				while (buff.rfind(']') == -1) {
					buff = buff.substr(0, buff.size() - 1);
					Limitations.push_back(std::stoi(buff));
					ifile >> buff;
				}
				// Добавление последнего элемента
				buff = buff.substr(0, buff.size() - 2);
				Limitations.push_back(std::stoi(buff));
			}

			// Считывание прибыли от реализации
			else if (buff == "\"A\":") {
				ifile >> buff;
				buff = buff.substr(1, buff.size());

				while (buff.rfind(']') == -1) {
					buff = buff.substr(0, buff.size() - 1);
					Aj.push_back(std::stoi(buff));
					ifile >> buff;
				}
				// Добавление последнего элемента
				buff = buff.substr(0, buff.size() - 2);
				Aj.push_back(std::stoi(buff));
			}

			// Считывание ограничений
			else if (buff == "\"K\":") {
				ifile >> buff;
				buff = buff.substr(1, buff.size());

				while (buff.rfind(']') == -1) {
					buff = buff.substr(0, buff.size() - 1);
					Kj.push_back(std::stod(buff));
					ifile >> buff;
				}
				// Добавление последнего элемента
				buff = buff.substr(0, buff.size() - 2);
				Kj.push_back(std::stod(buff));
			}

			// Считывание кол-ва ресурсов
			else if (buff == "\"max_res\":") {
				ifile >> buff;
				MaxResource = std::stoi(buff.substr(0, buff.size() - 1));
			}

			// Считывание функции
			else if (buff == "\"F\":") {
				ifile >> buff;
				function = buff.substr(1, buff.size() - 1);
			}
		}

		std::cout << "\n Считывание входного файла завершено...";
	}
	else {
		std::cout << "\n Не удалось открыть входной файл: " << path;
	}
}

void DynamicProgramm::PrintBellman() {

	std::cout << "\n\tξ\t\t fb[1]\t\t x[1]\t\t fb[2]\t\t x[2]\t\t fb[3]\t\t x[3]";
	std::cout << "\t\t fb[4]\t\t x[4]\t\t fb[5]\t\t x[5]\t\t fb[6]\t\t x[6]";

	for (int row = 0; row <= MaxResource; row++) {
		std::cout << "\n\t" << row + 1;
		for (int prod = 0; prod < CountProduct; prod++) {
			std::cout << "\t| " << std::setw(10) << bellmanFunction[row][prod];
			std::cout << std::setw(10) << " | " << noc[row][prod] << std::setw(10);
		}
	}
}

//--------------------------------------------------------------------------------||
