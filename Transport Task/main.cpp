﻿#include "pch.h"
#include "TransportTask.h"

std::string path = "Data/Input.txt";

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    TransportTask::Calc(path);
}
