//
// Created by Logib on 2024/12/10.
//
#include "ChartAnalyzer.h"

// template 1
bool func1(FILE *ChartFile){
    try {
        Chart A = getChartFile(ChartFile);
        return false;
    }
    catch (const char *ErrorLine){
        printf("%s\n", ErrorLine);
        return true;
    }
}

// template 2
bool func2(FILE *ChartFile){
    try {
        Chart A = getChartFile(ChartFile);
        return false;
    }
    catch (const ChartError& e){
        printf("%hd\n", *e.ErrorCode);
        return true;
    }
}

// template 3
bool func3(FILE *ChartFile){
    try {
        Chart A = getChartFile(ChartFile);
        return false;
    }
    catch (const ChartError& e){
        throw  ChartError(e); // or ChartError(*e.ErrorCode)
        return true; // maybe it is invalid
    }
}