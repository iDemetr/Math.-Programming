#pragma once
#include "pch.h"

#define potential std::pair<std::vector<double>*, std::vector<double>*>		// U - firts, V - second

//--------------------------------------------------------------------------------||

enum Direction { notFound = -2, Non = -1, up, down, left, right };

//--------------------------------------------------------------------------------||

struct cells {
	cells(double *val, double cost) {		
		Value = val;
		Cost = cost;
	}

	~cells() {
		//delete Value;
	}

	void setValue(double val) {
		if (Value != nullptr)
			*Value = val;
		else
			Value = new double (val);
	}

	double getValue() {
		return (Value == nullptr) ? 0 : *Value;
	}

	double* Value;	// Значение базиса
	double Cost;	// Вес ячейки
};


//--------------------------------------------------------------------------------||

static class TransportTask {
public:

	static void Calc(std::string);

private:

	//--------------------------------------------------------------------------------||

#pragma region --- Переменные ---

	/// <summary>
	/// Список производств.
	/// </summary>
	static std::vector<double> LProduction;
	/// <summary>
	/// Список потребителей.
	/// </summary>
	static std::vector<double> LConsumption;
	/// <summary>
	/// Список штрафов
	/// </summary>
	static std::vector<double> LPenalty;

	/// <summary>
	/// Базис.
	/// </summary>
	static std::vector<cells> Basis;

	static int CountProduct;
	static int CountConsumption;

	static int indexPotential;

	/// <summary>
	/// Индексы фиктивных поставщиков и потребителей
	/// </summary>
	static int dummyProd, dummyConsum;

#pragma endregion

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// Считывает входной файл в формате json
	/// </summary>
	static void ReadInput(std::string);

	/// <summary>
	/// Выводит базис
	/// </summary>
	static void PrintBasis();

	/// <summary>
	/// Выводит входные данные
	/// </summary>
	static void PrintInputs();
	
	static int getRow(int);

	static int getColumn(int);

	static int getMinCost(std::vector<cells>&, std::vector<double> &, std::vector<double> &);

	//--------------------------------------------------------------------------------||
	
	/// <summary>
	/// Проверят тип задачи и производит корректировку
	/// </summary>
	static bool Correction();

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// Алгоритм северо западного угла
	/// </summary>
	static void ANWA();

	/// <summary>
	/// Метод минимального элемента
	/// </summary>
	static void MME();

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// Формирует новый базис.
	/// </summary>
	static void NewBasis();
	
	/// <summary>
	/// Находит цепочку
	/// </summary>
	/// <returns></returns>
	static std::vector<int> getChain();
	
	/// <summary>
	/// Проверяет зачение целевой функции по базису.
	/// </summary>
	/// <returns></returns>
	static bool CheckBasis();

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// Вычисление методом потенциалов
	/// </summary>
	static potential DefinePotential();

	/// <summary>
	/// Проверка правильности решения
	/// true - если текущий базис оптимальный
	/// </summary>
	static bool CheckPotential(std::vector<double> &U, std::vector<double> &V);
	
	//--------------------------------------------------------------------------------||

};

