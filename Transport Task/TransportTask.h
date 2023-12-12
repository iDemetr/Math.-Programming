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

	double* Value;	// �������� ������
	double Cost;	// ��� ������
};


//--------------------------------------------------------------------------------||

static class TransportTask {
public:

	static void Calc(std::string);

private:

	//--------------------------------------------------------------------------------||

#pragma region --- ���������� ---

	/// <summary>
	/// ������ �����������.
	/// </summary>
	static std::vector<double> LProduction;
	/// <summary>
	/// ������ ������������.
	/// </summary>
	static std::vector<double> LConsumption;
	/// <summary>
	/// ������ �������
	/// </summary>
	static std::vector<double> LPenalty;

	/// <summary>
	/// �����.
	/// </summary>
	static std::vector<cells> Basis;

	static int CountProduct;
	static int CountConsumption;

	static int indexPotential;

	/// <summary>
	/// ������� ��������� ����������� � ������������
	/// </summary>
	static int dummyProd, dummyConsum;

#pragma endregion

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// ��������� ������� ���� � ������� json
	/// </summary>
	static void ReadInput(std::string);

	/// <summary>
	/// ������� �����
	/// </summary>
	static void PrintBasis();

	/// <summary>
	/// ������� ������� ������
	/// </summary>
	static void PrintInputs();
	
	static int getRow(int);

	static int getColumn(int);

	static int getMinCost(std::vector<cells>&, std::vector<double> &, std::vector<double> &);

	//--------------------------------------------------------------------------------||
	
	/// <summary>
	/// �������� ��� ������ � ���������� �������������
	/// </summary>
	static bool Correction();

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// �������� ������ ��������� ����
	/// </summary>
	static void ANWA();

	/// <summary>
	/// ����� ������������ ��������
	/// </summary>
	static void MME();

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// ��������� ����� �����.
	/// </summary>
	static void NewBasis();
	
	/// <summary>
	/// ������� �������
	/// </summary>
	/// <returns></returns>
	static std::vector<int> getChain();
	
	/// <summary>
	/// ��������� ������� ������� ������� �� ������.
	/// </summary>
	/// <returns></returns>
	static bool CheckBasis();

	//--------------------------------------------------------------------------------||

	/// <summary>
	/// ���������� ������� �����������
	/// </summary>
	static potential DefinePotential();

	/// <summary>
	/// �������� ������������ �������
	/// true - ���� ������� ����� �����������
	/// </summary>
	static bool CheckPotential(std::vector<double> &U, std::vector<double> &V);
	
	//--------------------------------------------------------------------------------||

};

