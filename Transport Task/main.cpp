﻿#include "pch.h"
#include "TransportTask.h"

std::string path = "Data/InputMyZMR.txt";

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    TransportTask::Calc(path);
    
    std::cout << "\n\n\n The end.\n";
    getchar();
}
