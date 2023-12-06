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

#pragma region --- ����� ---

	std::cout << "\n ���������� ������� ������������: " << CountProduct;
	std::cout << "\n ���������� ������� �����������: " << CountConsumption;

	std::cout << "\n ������ ������������ � �������: \n ";
	for (auto obj : LProduction) {
		std::cout << obj << " ";
	}
	std::cout << "\n ������ ����������� � �������: \n ";
	for (auto obj : LConsumption) {
		std::cout << obj << " ";
	}

	std::cout << "\n ������� ���������� ���������� ������: \n";
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

#pragma region --- ����� ---

	i = 0;
	std::cout << "\n ������� ����: \n";
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

		std::cout << "\n ���������� �������� �����...";

		while (ifile.eof(), ifile >> buff) {
			if (buff == "\"production\":") {
				ifile >> buff;

				buff = buff.substr(1, buff.size());
				while (buff.rfind(']') == -1) {
					buff = buff.substr(0, buff.size() - 1);
					LProduction.push_back(std::stod(buff));
					ifile >> buff;
				}
				// ���������� ���������� ��������
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

				// ���������� ���������� ��������
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
					// ���������� ���������� ��������
					buff = buff.substr(0, buff.size() - 2);
					Basis.push_back(cells(nullptr, std::stod(buff)));
					ifile >> buff;
				}
			}
		}

		std::cout << "\n ���������� �������� ����� ���������...";

		CountConsumption = LConsumption.size();
		CountProduct = LProduction.size();
	}
	else {
		std::cout << "\n �� ������� ������� ������� ����: " << path;
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

		// ����� �����������
		if (remains <= 0) {
			obj->setValue(*locProd + remains);		// ������� ���� �� ����������� �����������.	

			*locProd -= obj->getValue();
			*locConsum = 0;

			if (remains == 0) {
				locProd++;							// ������� � ����� ������������, ���� ������� �����������
				if (obj + 1 != Basis.end()) {

					// ����� ���������� 0 � ������� ��� ���������� ����������� 
					if ((obj + 1 - Basis.begin()) % CountProduct > 0)
						(obj + 1)->setValue(0);
					obj += CountConsumption;		// ����� � ������ �� ���� ���� - ���� ������ �����������
				}
			}

			obj++;
			locConsum++;							// ������� � ����� �����������
		}

		// ����� �� �����������
		else {
			obj->setValue(*locProd);				// ������� ���� �� ����������� �����������.
			*locConsum -= *locProd;
			*locProd = 0; locProd++;
			obj += CountConsumption;				// ����� � ������ �� ���� ���� - ���� ������ �����������
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

// ���������� ��� ������ ������ � ������� ������� � ���� ��� ��������� ������
void TransportTask::NewBasis() {

	std::vector<int> chain = getChain();

	// �������� ����������� ������� � ����������� ����� �� �������

}

std::vector<int> TransportTask::getChain() {
	auto locBasis(Basis);

	// ��� ��� ����
	std::vector<int> Chain;			// ������ �������� �� �������
	std::stack<int> state;			// ���� ���������

	Chain.push_back(indexPotential);

	int i = indexPotential;
	int direction = 0;
	bool isChain = false;

	int row = i / CountConsumption;
	int column = i % CountConsumption;

	/// <summary>
	/// ���� ������ �������
	/// </summary>
	while (!isChain) {

		// �������� �������� ���������� ������� - ������ ������ ����� ��� ������������ ���� ����������� � 
		// ��������� ��� �� ����������� �����, ����� ���������� ������ �� ���� �� ���(����� ���� �����)
		// �������� �������� �� �� ����� ���� ������ � ������ � �������, ��� ��� ��� �������� ������� ����.


		// ���� ������ ���� �����������
		while (direction != 4) {			// ������� �� ������� ���� �� 4 ���

			// ����� �����
			if (direction == 0) {

				bool isBasis = false;
				int ii = i - CountConsumption;
				row = ii / CountConsumption;

				// ���� �� ��������� �������
				while (row >= 0 && ii >= 0) {
					// ������������� ����������� ������ �������� � �������� ����������� ��������� contains
					if (contains(Chain, ii))
						break;

					isChain = ii == indexPotential;
					// ���� ��������� �������� ����������.					
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii -= CountConsumption;				// ��� �� ������� �����
					row = ii / CountConsumption;
				}

				// ���� ����� ��� ��������
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// ����� �����������
					break;
				}
				else {
					direction = 1;		// ������� �����������.
				}
			}

			// ����� ����
			else if (direction == 3) {

				bool isBasis = false;
				int ii = i + CountConsumption;
				row = ii / CountConsumption;

				// ���� �� ��������� ���
				while (row < CountProduct && ii < locBasis.size()) {
					// ������������� ����������� ������ �������� � �������� ����������� ��������� contains
					if (contains(Chain, ii))
						break;

					isChain = ii == indexPotential;
					// ���� ��������� �������� ����������.
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii += CountConsumption;				// ��� �� ������� �����
					row = ii / CountConsumption;
				}

				// ���� ����� ��� ��������
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// ����� �����������
					break;
				}
				else {
					direction++;		// ������� �����������.
				}
			}

			// ����� �����
			else if (direction == 2) {

				bool isBasis = false;
				int ii = i - 1;
				column = ii % CountConsumption;

				// ���� �� ���������� ����� �������
				while (column >= 0 && ii >= 0) {
					// ������������� ����������� ������ �������� � �������� ����������� ��������� contains
					if (contains(Chain, ii))
						break;
					isChain = ii == indexPotential;
					// ���� ��������� �������� ����������.
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii--;				// ��� �� ������� �����
					column = ii % CountConsumption;
				}

				// ���� ����� ��� ��������
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// ����� �����������
					break;
				}
				else {
					direction++;		// ������� �����������.
				}
			}

			// ����� ������
			else if (direction == 1) {
				bool isBasis = false;
				int ii = i + 1;
				column = ii % CountConsumption;

				// ���� �� ���������� ����� �������
				while (column < CountConsumption && ii < locBasis.size()) {
					// ������������� ����������� ������ �������� � �������� ����������� ��������� contains
					if (contains(Chain, ii))
						break;

					isChain = ii == indexPotential;
					// ���� ��������� �������� ����������.
					if (locBasis[ii].Value != nullptr || isChain) {
						isBasis = true;
						break;
					}

					ii++;				// ��� �� ������� �����
					column = ii % CountConsumption;
				}

				// ���� ����� ��� ��������
				if (isBasis) {
					i = ii;
					state.push(direction);
					direction = 0;		// ����� �����������
					break;
				}
				else {
					direction++;		// ������� �����������.
				}
			}
		}

		if (direction != 4) {
			Chain.push_back(i);
		}
		else {
			delete locBasis[i].Value;
			locBasis[i].Value = nullptr;
			Chain.pop_back();			// �������� �� �������
			state.pop();
			direction = state.top();	// ������� ����������� ��������� ������.
			i = Chain.back();
		}
	}
	Chain.pop_back();			// �������� ����� ������� ���������� �������� ����������.
}

bool TransportTask::CheckBasis() {
	double ansrew = 0;
	for (auto obj : Basis) {
		ansrew += obj.Cost * obj.getValue();
	}
	std::cout << "\n �������� �������: " << ansrew;
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
	// ���� ��� ������ ������������ - ����� ���������.
	return min >= 0;
}

//--------------------------------------------------------------------------------||

