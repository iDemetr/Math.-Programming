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

#pragma region --- ВЫВОД ВХОДНЫХ ДАННЫХ ---

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

	Correction();

	ANWA();

	PrintBasis();

	CheckBasis();
	std::cout << "\n Поиск оптимального решения.";

	int iter = 0, MaxIter = 100;
	while (iter++ < MaxIter) {
		std::cout << "\n ===============================================================";
		std::cout << " Итерация №" << iter << " ===";
		std::cout << "\n Расчёт потенциалов:\n ";
		potential potent = FindPotential();

#pragma region --- ВЫВОД ПОТЕНЦИАЛОВ ---

		int i = 0;
		for (auto x : *potent.first) {
			std::cout << "U[" << i++ << "] = " << x << '\t';
		}
		i = 0;
		std::cout << "\n ";
		for (auto x : *potent.second) {
			std::cout << "V[" << i++ << "] = " << x << '\t';
		}

#pragma endregion

		if (!CheckPotential(*potent.first, *potent.second)) {
			
			std::cout << "\n Условие оптимальности не выполняется";
			std::cout << "\n В базис вводится X(" << indexPotential / CountProduct << "," << indexPotential % CountConsumption << ")";

			NewBasis();

			PrintBasis();

			CheckBasis();
		}
		else {
			std::cout << "\n Выполнилось условие оптимальности.";
			break;
		}
	}
	std::cout << "\n ==============================================================";
	std::cout << "\n Количество сделанных шагов:" << iter;
	CheckBasis();
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

void TransportTask::Correction() {
	int sumProd = 0, sumConsum = 0;

	for (auto x : LProduction) {
		sumProd += x;
	}

	for (auto x : LConsumption) {
		sumConsum += x;
	}

	if (sumConsum == sumProd) {
		std::cout << "\n Закрытая транспортная задача.";
	}
	else if (sumConsum < sumProd) {
		std::cout << "\n Открытая транспортная задача с перепроизводством.";
	}
	else if (sumProd < sumConsum) {
		std::cout << "\n Открытая транспортная задача с недопроизводством.";
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
					// дальше первого столбца
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

	std::cout << "\n Цепочка:\n ";
	for (int x : chain) {
		std::cout << "(" << x / CountProduct << ", " << x % CountProduct << ")";
	}

#pragma region --- Поиск минимального элемента и сдвиг по цепочке ---

	int min = INT32_MAX;

	for (auto cell = chain.begin() + 1; cell != chain.end(); cell++) {
		min = (min > Basis[*cell].getValue()) ? Basis[*cell].getValue() : min;
	}

	bool isFirts = false;
	for (int i = 0; i < chain.size(); i++) {
		Basis[chain[i]].setValue((i % 2 > 0) ? Basis[chain[i]].getValue() - min : Basis[chain[i]].getValue() + min);
		if (!isFirts && *Basis[chain[i]].Value == 0 && chain[i] != indexPotential) {
			std::cout << "\n Из базиса выводится элемент X(" << chain[i] / CountProduct << "," << chain[i] % CountConsumption << ") = " << min;
			delete Basis[chain[i]].Value;
			Basis[chain[i]].Value = nullptr;
			isFirts = true;
		}
	}

#pragma endregion	

}

std::vector<int> TransportTask::getChain() {

	std::cout << "\n Поиск цепочки.";

	auto locBasis(Basis);

	// ВОТ ТУТ ЖОПА
	std::vector<int> Chain;							// Список индексов из цепочки
	std::stack<std::array<int, 4>> state;			// Стек состояний

	Chain.push_back(indexPotential);

	int i = indexPotential;
	bool isChain = false;

	Direction fromDir = Non;

	int row = i / CountConsumption;
	int rowIter = 0;
	int column = i % CountConsumption;
	int columnIter = 0;

	int counterIter = 0;
	// Вектор с базисными индексами в различных направлениях
	std::array<int, 4> ListDir{-1, -1, -1, -1};

	/// <summary>
	/// Цикл перехода к новой базисной ячейке
	/// </summary>
	while (!isChain && counterIter < CountConsumption * CountConsumption) {
		counterIter++;

		column = i % CountConsumption;
		row = i / CountConsumption;

		// Улучшить скорость нахождения цепочек - ввести список ячеек при сканировании всех направлений и		++
		// проверять его на добавляемый базис, иначе переходить вглубь на одну из них(почти тоже самое)			++
		// Добавить проверку на не более двух вершин в строке и столбце, что ещё раз позволит скипать шаги.

		// Обход вверх
		if(fromDir != up){
			bool isBasis = false;
			int ii = i - CountConsumption;			// Временный итератор по столбцу
			columnIter = ii % CountConsumption;

			// Пока не достигнут потолок
			while (columnIter == column && ii >= 0) {
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
				rowIter = ii / CountConsumption;
				columnIter = ii % CountConsumption;
			}

			// Если базис был встречен
			if (isBasis) {
				if (isChain) {
					i = ii;				// Цепочка найдена - остановка поисков
					break;
				}
				else {
					ListDir[up] = ii;	// В направлении вверх записываем индекс следующего шага					
				}
			}
		}

		// Обход вниз
		if(fromDir != down){
			bool isBasis = false;
			int ii = i + CountConsumption;			// Временный итератор по столбцу
			columnIter = ii % CountConsumption;

			// Пока не достигнут пол
			while (columnIter == column && ii < locBasis.size()) {
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
				rowIter = ii / CountConsumption;
				columnIter = ii % CountConsumption;
			}

			// Если базис был встречен
			if (isBasis) {
				if (isChain) {
					i = ii;				// Цепочка найдена - остановка поисков
					break;
				}
				else {
					ListDir[down] = ii;	// В направлении вверх записываем индекс следующего шага					
				}
			}
		}

		// Обход влево
		if(fromDir != left){
			bool isBasis = false;
			int ii = i - 1;							// Временный итератор по строке			
			rowIter = ii / CountConsumption;

			// Пока не достигнута левая границы
			while (rowIter == row && ii >= 0) {
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
				columnIter = ii % CountConsumption;
				rowIter = ii / CountConsumption;
			}

			// Если базис был встречен
			if (isBasis) {
				if (isChain) {
					i = ii;				// Цепочка найдена - остановка поисков
					break;
				}
				else {
					ListDir[left] = ii;	// В направлении вверх записываем индекс следующего шага					
				}
			}
		}

		// Обход вправо
		if(fromDir != right){
			bool isBasis = false;
         	int ii = i + 1;							// Временный итератор по строке
			rowIter = ii / CountConsumption;
			
			// Пока не достигнута левая границы
			while (rowIter == row && ii < locBasis.size()) {
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
				columnIter = ii % CountConsumption;
				rowIter = ii / CountConsumption;
			}

			// Если базис был встречен
			if (isBasis) {
				if (isChain) {
					i = ii;				// Цепочка найдена - остановка поисков
					break;
				}
				else {
					ListDir[right] = ii;	// В направлении вверх записываем индекс следующего шага					
				}
			}
		}


		// Если цепочка не найдена - но есть куда ходить
		if (ListDir[0] != -1 || ListDir[1] != -1 || ListDir[2] != -1 || ListDir[3] != -1) {

			Direction j = up;
			while (j < 4) {
				if (ListDir[j] != -1) {
					Chain.push_back(ListDir[j]);
					i = ListDir[j];
					ListDir[j] = -1;	// Отмечается направление, в котором будет шаг
					break;
				}
				j = (Direction)(j + 1);
			}
			
			// Костыли
			// На следующую итерацию блокируется ход назад.
			switch (j)
			{
			case up:
				fromDir = down;
				break;
			case down:
				fromDir = up;
				break;
			case left:
				fromDir = right;
				break;
			case right:
				fromDir = left;
				break;
			default:
				break;
			}

			state.push(ListDir);

			ListDir = { -1, -1, -1, -1 };
		}

		// Если все пути пройдены - шаг назад
		else {
			//delete locBasis[i].Value;
			locBasis[i].Value = nullptr;

			Chain.pop_back();			// Удаление из цепочки
			i = Chain.back();

			state.pop();
			ListDir[up] = state.top()[up];			// Возврат предыдущего состояния обхода.
			ListDir[down] = state.top()[down];		// Возврат предыдущего состояния обхода.
			ListDir[left] = state.top()[left];		// Возврат предыдущего состояния обхода.
			ListDir[right] = state.top()[right];	// Возврат предыдущего состояния обхода.
		}
	}
	//Chain.pop_back();			// Удаление дубля индекса добавления базисной переменной.

	std::vector<int> newChain;
	newChain.push_back(Chain[0]);
	newChain.push_back(Chain[1]);

	for (int i = 2; i < Chain.size(); i++) {
		if (Chain[i - 1] % CountProduct == Chain[i] % CountProduct &&
			Chain[i - 2] % CountProduct == Chain[i] % CountProduct) {
			newChain[newChain.size() - 1] = Chain[i];			// Перезапись дубля			
		}
		else {
			newChain.push_back(Chain[i]);
		}
	}

	Chain = newChain;

	std::cout << "\n Цепочка найдена за " << counterIter << " шагов.";

	return Chain;
}

bool TransportTask::CheckBasis() {
	double ansrew = 0;
	for (auto obj : Basis) {
		ansrew += obj.Cost * obj.getValue();
	}
	std::cout << "\n Значение целевой функции: " << ansrew;
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
		// Проблема на итерации 2 - пропадает связн
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
			else if (*u != LONG_MAX && *v != LONG_MAX) {
				if (iter + CountConsumption < Basis.size())
					iter += CountConsumption;	// Переход на некст строку
				else
					iter++;
			}
			else {
				*u = 0;
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
	//std::set<double> set;

	std::cout << "\n Оценки свободных переменных:\n ";

	while (iter < Basis.size()) {
		if (Basis[iter].Value == nullptr) {

			u = &U[iter % CountProduct];
			v = &V[iter / CountProduct];

			if (min > Basis[iter].Cost - *u - *v) {
				iterMin = iter;
				min = Basis[iter].Cost - *u - *v;
			}
			std::cout << "ΔC[" << iter / CountProduct << "," << iter % CountConsumption << "] = " << Basis[iter].Cost - *u - *v << "\n ";
			iter++;
		}
		else
			iter++;
	}

	delete & U;
	delete & V;


	indexPotential = iterMin;
	// Если все оценки положительны - базис оптимален.
	return min >= 0;
}

//--------------------------------------------------------------------------------||

