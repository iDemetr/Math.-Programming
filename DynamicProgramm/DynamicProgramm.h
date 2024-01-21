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

#pragma region --- ���������� ---

	static int nmax;
	static double bmax;

#pragma region --- �������� ���������� ---

	/// <summary>
	/// ������ � ��������
	/// </summary>
	static std::string function;

	/// <summary>
	/// ���������� ����������� �������
	/// </summary>
	/// <param name=""></param>
	static int MaxResource;
	/// <summary>
	/// ���������� ����� ������������ ���������
	/// </summary>
	static int CountProduct;

	/// <summary>
	/// ������ �������������� �����������
	/// </summary>
	static std::vector<int> Limitations;

	/// <summary>
	/// ����-�� ����������� (����)
	/// </summary>
	static std::vector <double> Kj;
	/// <summary>
	/// ������� �� ���������� ���������
	/// </summary>
	static std::vector <double> Aj;
	/// <summary>
	/// ����� �������� ���������
	/// </summary>

#pragma endregion
	
	/// <summary>
	/// ������� ����������� ����������.
	/// </summary>
	static std::vector<std::vector<int>> xj;
	/// <summary>
	/// ������� ������� ����������� ����������.
	/// </summary>
	static std::vector<std::vector<int>> noc;
	/// <summary>
	/// ������� �������� ������� ��������.
	/// </summary>
	static std::vector<std::vector<double>> bellmanFunction;

#pragma endregion
	
	//--------------------------------------------------------------------------------||

	/// <summary>
	/// ������ ������� ��������
	/// </summary>
	static void FuncBellman();

	/// <summary>
/// ������ �������� �������
/// </summary>
/// <param name="nper">����� ����������</param>
/// <param name="res">���������� ������</param>
/// <returns></returns>
	static double Func(int, int);

	/// <summary>
	/// ���������� ������������ �������� � �������
	/// </summary>
	static void getMax(std::vector<double>, int);

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// ��������� ������� ���� � ������� json
	/// </summary>
	static void ReadInput(const std::string);

	/// <summary>
	/// ��������� ������� ���� � ������� json
	/// </summary>
	static void PrintBellman();

	//--------------------------------------------------------------------------------||

};

//#endif //DynamicProgramm_H#pragma once