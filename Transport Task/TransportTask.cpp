#include "pch.h"
#include "TransportTask.h"

//--------------------------------------------------------------------------------||

std::vector<cells> TransportTask::Basis;
std::vector<double> TransportTask::LProduction;
std::vector<double> TransportTask::LConsumption;

int TransportTask::CountProduct = 0;
int TransportTask::CountConsumption = 0;
int TransportTask::indexPotential = 0;

//--------------------------------------------------------------------------------||

void TransportTask::Calc(std::string path) {

	ReadInput(path);

#pragma region --- ВЫВОД ---

	std::cout << "\n Количество пунктов производства: " << CountProduct;
	std::cout << "\n Количество пунктов потребления: " << CountConsumption;

	std::cout << "\n Объёмы производства в пунктах: \n ";
	for (auto obj : LProduction) {
		std::cout << obj << " ";
	}
	std::cout << "\n Объёмы потребления в пунктах: \n ";
	for (auto obj : LConsumption) {
		std::cout << obj << " ";
	}

	std::cout << "\n Матрица стоимостей выполнения задачи: \n";
	int i = 0;
	for (auto obj : Basis) {
		if (i++ != CountProduct) {
			std::cout << std::setw(8) << obj.Cost;
		}
		else {
			i = 1;
			std::cout << "\n" << std::setw(8) << obj.Cost;
		}
	}

#pragma endregion

	ANWA();

#pragma region --- ВЫВОД ---

	i = 0;
	std::cout << "\n Опорный план: \n";
	for (auto obj : Basis) {
		if (i++ != CountProduct) {
			std::cout << std::setw(8) << obj.getValue();
		}
		else {
			i = 1;
			std::cout << "\n" << std::setw(8) << obj.getValue();
		}
	}

#pragma endregion

	CheckBasis();

	int iter = 1, MaxIter = 100;
	//while (iter++ <= MaxIter) {
	potential potent = FindPotential();

	if (!CheckPotential(*potent.first, *potent.second)) {
		NewBasis();
		CheckBasis();
	}
	//else break;
//}
}

//--------------------------------------------------------------------------------||

void TransportTask::ReadInput(std::string path) {
	std::ifstream ifile(path);

	if (ifile.is_open()) {
		std::string buff;

		std::cout << "\n Считывание входного файла...";

		while (ifile.eof(), ifile >> buff) {
			if (buff == "\"production\":") {
				ifile >> buff;

				buff = buff.substr(1, buff.size());
				while (buff.rfind(']') == -1) {
					buff = buff.substr(0, buff.size() - 1);
					LProduction.push_back(std::stod(buff));
					ifile >> buff;
				}
				// Добавление последнего элемента
				buff = buff.substr(0, buff.size() - 2);
				LProduction.push_back(std::stod(buff));
			}

			else if (buff == "\"consumption\":") {
				ifile >> buff;

				buff = buff.substr(1, buff.size());

				while (buff.rfind(']') == -1) {
					buff = buff.substr(0, buff.size() - 1);
					LConsumption.push_back(std::stod(buff));
					ifile >> buff;
				}

				// Добавление последнего элемента
				buff = buff.substr(0, buff.size() - 2);
				LConsumption.push_back(std::stod(buff));
			}

			else if (buff == "\"cost\":") {
				ifile >> buff >> buff;
				while (buff[0] != ']' || buff.size() > 1) {
					buff = buff.substr(1, buff.size());
					while (buff.rfind(']') == -1) {
						buff = buff.substr(0, buff.size() - 1);
						Basis.push_back(cells(nullptr, std::stod(buff)));
						ifile >> buff;
					}
					// Добавление последнего элемента
					buff = buff.substr(0, buff.size() - 2);
					Basis.push_back(cells(nullptr, std::stod(buff)));
					ifile >> buff;
				}
			}
		}

		std::cout << "\n Считывание входного файла завершено...";

		CountConsumption = LConsumption.size();
		CountProduct = LProduction.size();
	}
	else {
		std::cout << "\n Не удалось открыть входной файл: " << path;
	}
}

//--------------------------------------------------------------------------------||

void TransportTask::ANWA() {

	auto locLProduction(LProduction);
	auto locLConsumption(LConsumption);

	auto locProd = locLProduction.begin();
	auto locConsum = locLConsumption.begin();
	auto obj = Basis.begin();

	double remains(0);

	while (obj != Basis.end()) {

		remains = *locConsum - *locProd;

		// Спрос удовлетворён
		if (remains <= 0) {
			obj->setValue(*locProd + remains);		// Сколько ушло от предприятия потребителю.	

			*locProd -= obj->getValue();
			*locConsum = 0;

			if (remains == 0) {
				locProd++;							// Переход к некст производству, если ресурсы закончились
				if (obj + 1 != Basis.end()) {

					// Занос фиктивного 0 в матрицу для вычисления потенциалов 
					if ((obj + 1 - Basis.begin()) % CountProduct > 0)
						(obj + 1)->setValue(0);
					obj += CountConsumption;		// Сдвиг в списке на слой ниже - слой нового потребителя
				}
			}

			obj++;
			locConsum++;							// Переход к некст потребителю
		}

		// Спрос не удовлетворён
		else {
			obj->setValue(*locProd);				// Сколько ушло от предприятия потребителю.
			*locConsum -= *locProd;
			*locProd = 0; locProd++;
			obj += CountConsumption;				// Сдвиг в списке на слой ниже - слой нового потребителя
		}
	}
}

void TransportTask::MME() {

}

//--------------------------------------------------------------------------------||

template <typename T>
bool contains(std::vector<T> vec, T value) {
	auto iter = vec.begin();
	while (++iter != vec.end()) {
		if (*iter == value)
			return true;
	}
	return false;
}

//--------------------------------------------------------------------------------||

// Запоминать для каждой строки и столбца сколько у него уже встречено вершин
void TransportTask::NewBasis() {

	std::vector<int> chain = getChain();

	// Находить минимальный элемент и осуществить сдвиг по цепочке

}

std::vector<int> TransportTask::getChain() {
	auto locBasis(Basis);

	// ВОТ ТУТ ЖОПА
	std::vector<int> Chain;			// Список индексов из цепочки
	std::stack<int> state;			// Стек состояний

	Chain.push_back(indexPotential);

	int i = indexPotential;
	int direction = 0;
	bool isChain = false;

	int row = i / CountConsumption;
	int column = i % CountConsumption;

	/// <summary>
	/// Цикл поиска цепочек
	/// </summary>
	while (!isChain) {

		// Улучшить скорость нахождения цепочек - ввести список ячеек при сканировании всех направлений и 
		// проверять его на добавляемый базис, иначе переходить вглубь на одну из них(почти тоже самое)
		// Добавить проверку на не более двуъ вершин в строке и столбце, что ещё раз позволит скипать шаги.


		// Цикл обхода всех направлений
		while (direction != 4) {			// Зменить на обычный цикл до 4 раз

			// Обход вверх
			if (direction == 0) {

				bool isBasis = false;
				int ii = i - CountConsumption;
				row = ii / CountConsumption;

				// Пока не достигнут потолок
				while (row >= 0 && ii >= 0) {
					// Автоматически исключается случай хождения в обратном направлении благодаря contains
					if (contains(Chain, ii))
						break;

					isChain = ii == indexPotential;
					// Если встречена базисная переменная.					
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii -= CountConsumption;				// Шаг по матрице вверх
					row = ii / CountConsumption;
				}

				// Если базис был встречен
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// сброс направлений
					break;
				}
				else {
					direction = 1;		// отметка направления.
				}
			}

			// Обход вниз
			else if (direction == 3) {

				bool isBasis = false;
				int ii = i + CountConsumption;
				row = ii / CountConsumption;

				// Пока не достигнут пол
				while (row < CountProduct && ii < locBasis.size()) {
					// Автоматически исключается случай хождения в обратном направлении благодаря contains
					if (contains(Chain, ii))
						break;

					isChain = ii == indexPotential;
					// Если встречена базисная переменная.
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii += CountConsumption;				// Шаг по матрице вверх
					row = ii / CountConsumption;
				}

				// Если базис был встречен
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// сброс направлений
					break;
				}
				else {
					direction++;		// отметка направления.
				}
			}

			// Обход влево
			else if (direction == 2) {

				bool isBasis = false;
				int ii = i - 1;
				column = ii % CountConsumption;

				// Пока не достигнута левая границы
				while (column >= 0 && ii >= 0) {
					// Автоматически исключается случай хождения в обратном направлении благодаря contains
					if (contains(Chain, ii))
						break;
					isChain = ii == indexPotential;
					// Если встречена базисная переменная.
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii--;				// Шаг по матрице вверх
					column = ii % CountConsumption;
				}

				// Если базис был встречен
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// сброс направлений
					break;
				}
				else {
					direction++;		// отметка направления.
				}
			}

			// Обход вправо
			else if (direction == 1) {
				bool isBasis = false;
				int ii = i + 1;
				column = ii % CountConsumption;

				// Пока не достигнута левая границы
				while (column < CountConsumption && ii < locBasis.size()) {
					// Автоматически исключается случай хождения в обратном направлении благодаря contains
					if (contains(Chain, ii))
						break;

					isChain = ii == indexPotential;
					// Если встречена базисная переменная.
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii++;				// Шаг по матрице вверх
					column = ii % CountConsumption;
				}

				// Если базис был встречен
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// сброс направлений
					break;
				}
				else {
					direction++;		// отметка направления.
				}
			}
		}

		if (direction != 4) {
			Chain.push_back(i);
		}
		else {
			delete locBasis[i].Value;
			locBasis[i].Value = nullptr;
			Chain.pop_back();			// Удаление из цепочки
			state.pop();
			direction = state.top();	// Возврат предыдущего состояния обхода.
			i = Chain.back();
		}
	}
	Chain.pop_back();			// Удаление дубля индекса добавления базисной переменной.
}

bool TransportTask::CheckBasis() {
	double ansrew = 0;
	for (auto obj : Basis) {
		ansrew += obj.Cost * obj.getValue();
	}
	std::cout << "\n Значение функции: " << ansrew;
	return 0;
}

//--------------------------------------------------------------------------------||

potential TransportTask::FindPotential() {
	double* u;
	double* v;

	auto* U = new std::vector<double>();
	for (size_t i = 0; i < CountProduct; i++) {
		U->push_back(LONG_MAX);
	}
	(*U)[0] = 0;

	auto* V = new std::vector<double>();
	for (size_t i = 0; i < CountConsumption; i++) {
		V->push_back(LONG_MAX);
	}

	int iter = 0;
	while (iter < Basis.size()) {
		if (Basis[iter].Value != nullptr) {

			int a = iter / CountProduct;
			int b = iter % CountProduct;

			u = &(*U)[a];
			v = &(*V)[b];

			if (*u == LONG_MAX && *v != LONG_MAX) {
				*u = Basis[iter].Cost - *v;
				iter++;
			}
			else if (*v == LONG_MAX && *u != LONG_MAX) {
				*v = Basis[iter].Cost - *u;
				iter++;
			}
		}
		else
			iter++;
	}

	return  potential(U, V);
}

bool TransportTask::CheckPotential(std::vector<double>& U, std::vector<double>& V) {

	double* u;
	double* v;
	int iter = 0;

	double min = LONG_MAX;
	int iterMin;
	std::set<double> set;

	while (iter < Basis.size()) {
		if (Basis[iter].Value == nullptr) {

			u = &U[iter % CountProduct];
			v = &V[iter / CountProduct];

			if (min > Basis[iter].Cost - *u - *v) {
				iterMin = iter;
				min = Basis[iter].Cost - *u - *v;
			}
			iter++;
		}
		else
			iter++;
	}

	indexPotential = iterMin;
	// Если все оценки положительны - базис оптимален.
	return min >= 0;
}

//--------------------------------------------------------------------------------||

