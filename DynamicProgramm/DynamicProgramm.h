#pragma once
#include "pch.h"

//#ifndef DynamicProgramm_H
//#define DynamicProgramm_H

//--------------------------------------------------------------------------------||

static class DynamicProgramm {

public:

	static void Calc(const std::string);

private:

	//--------------------------------------------------------------------------------||

#pragma region --- Переменные ---

	static int nmax;
	static double bmax;

#pragma region --- Выходные переменные ---

	/// <summary>
	/// Строка с функцией
	/// </summary>
	static std::string function;

	/// <summary>
	/// Количество дефицитного ресурса
	/// </summary>
	/// <param name=""></param>
	static int MaxResource;
	/// <summary>
	/// Количество видов производимой продукции
	/// </summary>
	static int CountProduct;

	/// <summary>
	/// Список ограничивающих ограничений
	/// </summary>
	static std::vector<int> Limitations;

	/// <summary>
	/// Коэф-ты потребления (проц)
	/// </summary>
	static std::vector <double> Kj;
	/// <summary>
	/// Прибыль от реализации продукции
	/// </summary>
	static std::vector <double> Aj;
	/// <summary>
	/// Объем выпускаа продукции
	/// </summary>

#pragma endregion
	
	/// <summary>
	/// Таблица оптимальных управлений.
	/// </summary>
	static std::vector<std::vector<int>> xj;
	/// <summary>
	/// Таблица условно оптимальных управлений.
	/// </summary>
	static std::vector<std::vector<int>> noc;
	/// <summary>
	/// Таблица значений функции Беллмана.
	/// </summary>
	static std::vector<std::vector<double>> bellmanFunction;

#pragma endregion
	
	//--------------------------------------------------------------------------------||

	/// <summary>
	/// Расчет функции Беллмана
	/// </summary>
	static void FuncBellman();

	/// <summary>
/// Расчет значения функции
/// </summary>
/// <param name="nper">Номер переменной</param>
/// <param name="res">Выделенный ресурс</param>
/// <returns></returns>
	static double Func(int, int);

	/// <summary>
	/// Возвращает максимальное значение в столбце
	/// </summary>
	static void getMax(std::vector<double>, int);

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// Считывает входной файл в формате json
	/// </summary>
	static void ReadInput(const std::string);

	/// <summary>
	/// Считывает входной файл в формате json
	/// </summary>
	static void PrintBellman();

	//--------------------------------------------------------------------------------||

};

//#endif //DynamicProgramm_H#pragma once