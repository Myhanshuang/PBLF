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

#ifndef KEYRHYTHM_CHARTKMP_H
#include "ChartKMP.h"
#endif

/**
 * @brief　to get chart
 *
 * @param chartFile
 * @return Chart
 ***/
Chart getChart(FILE *chartFile);

extern int MaxOffset[8];
void changeJudgment(int *NewJudge);

#endif //KEYRHYTHM_CHARTWORK_H
