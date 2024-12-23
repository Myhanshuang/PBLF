//
// Created by Logib on 2024/12/11.
//

/**
 * @author \b Logib
 *
 ***/

#ifndef KEYRHYTHM_CHARTWORK_H
#define KEYRHYTHM_CHARTWORK_H

#ifndef KEYRHYTHM_CHART_H
#include "Chart.h"
#endif

#ifndef KEYRHYTHM_CHARTREAD_H
#include "ChartRead.h"
#endif

/**
 * @brief　to get chart
 *
 * @param chartFile
 * @param NowPlay
 ***/
void getChart(FILE *chartFile, Chart& NowPlay);


/**
 * @brief　to get chart meta data
 *
 * @param chartFile
 * @param songTitle
 * @param Artist
 ***/
void getChartMeta(FILE *chartFile, wchar_t *songTitle, wchar_t *Artist);


void changeJudgment(const int *NewJudge);

#endif //KEYRHYTHM_CHARTWORK_H
