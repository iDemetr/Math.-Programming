#include "pch.h"
#include "DynamicProgramm.h";

const std::string path = "Data/Input.txt";

//using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    DynamicProgramm::Calc(path);

    std::cout << "\n\n\n The end.\n";
    getchar();
}