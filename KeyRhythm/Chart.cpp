//
// Created by Logib on 2024/12/11.
//

#ifndef KEYRHYTHM_CHART_H
#include "Chart.h"
#endif

/**
 * @class Chart
 * @brief initialize Chart with param
 * @param Keys
 ***/
Chart :: Chart(const int Keys){
    this ->Column = Keys;
    this ->ChartHead = nullptr;
}

/**
 * @class Chart
 * @brief constructor of Chart
 ***/
Chart :: Chart(){
     this ->Column = 4;
     this ->ChartHead = nullptr;
}

/**
 * @class Chart
 * @brief destructor of Chart
 ***/
Chart :: ~Chart(){
    Chart :: Measure *temp = ChartHead, *preMea = nullptr;
    if (temp != nullptr){
        while(temp -> NxtMea != nullptr)
        {
            preMea = temp;
            temp = preMea ->NxtMea;
            preMea ->~Measure();
            delete preMea;
            return ;
        }
        delete ChartHead;
    }
}

/**
 * @class Measure
 * @brief initialize Measure with param
 * @param Keys
 ***/
Chart :: Measure :: Measure(const int Keys){
    this ->timeStamp = 0;
    this ->timeTable = nullptr;
    this ->NxtMea = nullptr;
    this ->Bar=new char [Keys];
}

/**
 * @class Measure
 * @brief constructor of Measure
 ***/
Chart :: Measure :: Measure(){
    this ->timeStamp = 0;
    this ->timeTable = nullptr;
    this ->NxtMea = nullptr;
    this ->Bar=new char [4];
}

/**
 * @class Measure
 * @brief destructor of Measure
 ***/
Chart :: Measure :: ~Measure(){
    delete[] this ->Bar;
    delete[] this ->timeTable;
    this ->Bar = nullptr;
    this ->timeTable = nullptr;
}

/**
 * @class Measure
 * @brief initialize new Measure with old one
 * @param M
 ***/
Chart :: Measure :: Measure(const Chart :: Measure &M) {
    this ->ifHold = M.ifHold;
    this ->timeStamp = M.timeStamp;
    short j = sizeof (M.Bar);
    this ->Bar = new char [j];
    memcpy(this ->Bar, M.Bar, j);

    if (M.ifHold){
        this ->timeTable = new int [j];
        memcpy(this ->timeTable, M.timeTable, j * sizeof (int ));
    }
    else this ->timeTable = nullptr;
    this ->NxtMea = M.NxtMea;
}

/**
 * @class Result
 * @brief constructor of Result
 ***/
Result :: Result(){
    this ->Offset = -1;
    this ->Column = -1;
    this ->Judgment = -1;
    this ->ifHoldEnd = false;
}

/**
 * @class Result
 * @brief initialize new Result with old one
 * @param R
 */
Result :: Result(const Result &R) {
    this ->Offset = R.Offset;
    this ->Column = R.Column;
    this ->Judgment = R.Judgment;
    this ->ifHoldEnd = R.ifHoldEnd;
}


Result :: Result(short Key, int Dis, bool HoldEnd) {
    this ->Column = Key;
    this ->Offset = Dis;
    this ->ifHoldEnd = HoldEnd;
    this ->Judgment =
}