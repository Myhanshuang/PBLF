//
// Created by Logib on 2024/12/11.
//

/**
 * @author \b Logib
 *
 ***/

#ifndef KEYRHYTHM_CHART_H
#include "../inc/Chart.h"
#endif
#include<QDebug>

/**
 * @brief　to get chart
 *
 * @param chartFile
 * @param NowPlay
 ***/
void getChart(FILE *chartFile, Chart &NowPlay){

    //打开谱面文件，我这段先写malody谱面的情况，osu谱面以后再考虑，因为两种的格式不大一样
    if (chartFile == nullptr) throw ChartError(0);
    short t = 0;
/*
 * 打算采用时间戳取代具体的节拍，即导入时算出时间戳
 */
    //获取谱面元数据
    /*
     * 该位置js代码如下（.mc文件中无换行与缩进，该代码为样例）
     * "meta": {
		    "$ver": 0,
		    "creator": "红色电音极地大冲击",
		    "background": "BG (2).jpg",
		    "version": "4K HI HandStream",
		    "id": 0,
		    "mode": 0,
		    "time": 1702658284,
		    "song": {
		    	"title": "我想对你说Baby(Yu vs. CTM Radio Mix)",
			    "artist": "VAVA",
		    	"id": 0
		    },
		    "mode_ext": {
			    "column": 4,
			    "bar_begin": 0
		    }
	    },
     */
    if ( !getKeyWord(chartFile, "meta") ) throw ChartError(1);
    if ( !getKeyWord(chartFile, "song") ) throw ChartError(1);
    if ( !getKeyWord(chartFile, "title") ) throw ChartError(1);
    getWords_w(chartFile, NowPlay.songTitle);
    if ( !getKeyWord(chartFile, "artist") ) throw ChartError(1);
    getWords_w(chartFile, NowPlay.Artist);
    if ( !getKeyWord(chartFile, "mode_ext") ) throw ChartError(2);
    if ( !getKeyWord(chartFile, "column") ) throw ChartError(2);
    getValueShort(chartFile, t);
    NowPlay.Column = t;
    //获取bpm，同时确认每小节的时长
    /*
     * 该位置js代码如下（.mc文件中无换行与缩进，该代码为样例）
     * "time": [{
		    "beat": [0, 0, 1],
		    "bpm": 175.0
	    }],
     */


    if ( !getKeyWord(chartFile, "time") ) throw ChartError(3);
    if ( !getKeyWord(chartFile, "beat") ) throw ChartError(3);
    do {
        getValueShort(chartFile, t);
    }
    while (!t);
    NowPlay.everyBeat = FloatMinute / (float )t;

    if ( !getKeyWord(chartFile, "bpm") ) throw ChartError(3);
    getValueLDouble(chartFile, NowPlay.beatsPerMinute);
    NowPlay.everyBeat = NowPlay.everyBeat  / (((double)((int)(NowPlay.beatsPerMinute * 10000)))/10000.0l);

    //接下来写入谱面文件到Chart内
    /*
     * 该位置js代码如下（.mc文件中无换行与缩进，该代码为样例）
     * "note": [{
		    "beat": [1, 0, 8],
		    "column": 1
	    }, {
		    "beat": [1, 0, 8],
		    "column": 0
	    },
     */

    if ( !getKeyWord(chartFile, "note") ) throw ChartError(4);

    /* int tBar,  tBeat, tDivision, tTimeStamp, tKey;
     * tChart[0]  [1]   [2]        [3]         [8]
     * endBar, endBeat, endDivision, endTimeStamp,
     * [4]     [5]      [6]          [7]
     */
    int *tChart = new int [9];
    memset(tChart, 0, 9*sizeof (int ));
    const Chart :: Measure temple(NowPlay.Column);

    void *ptrMea = (void* )new char[sizeof(temple)];
    auto tMeasure = new(ptrMea) Chart :: Measure(NowPlay.Column);
    Chart :: Measure *preMea = nullptr;

    /*tMeasure ->timeStamp = 0;
    tMeasure ->Bar = new char [NowPlay.Column];
    tMeasure ->NxtMea = nullptr;
    tMeasure ->timeTable = nullptr;
    tMeasure ->ifHold = false;*/

    while (!feof(chartFile)){
        if ( !getKeyWord(chartFile, "beat") ){
            if (!feof(chartFile)) throw ChartError(5);
            break ;
        }

        for (short i = 0; i < 3; ++i) getValueInt(chartFile, tChart[i]);
        ///< @brief to get the beat info and turn it into timestamp
        tChart[3] = BeatToTime(NowPlay.everyBeat, tChart[0], tChart[1], tChart[2]);
        // tChart[3] = (int )(everyBeat * (float )(tChart[0] - 1) + everyBeat / (float )tChart[2] * (float )tChart[1] + 0.5f);
        if (tMeasure ->timeStamp > tChart[3]) break ;
        if (tMeasure ->timeStamp < tChart[3]){
            ///< @brief to end the previous node and connect to new next node
            if (NowPlay.ChartHead == nullptr) NowPlay.ChartHead = tMeasure;
            ///@brief if the list is empty, then the new become the first

            preMea = tMeasure;
            ptrMea = (void* )new char[sizeof(temple)];
            tMeasure = new(ptrMea) Chart :: Measure(NowPlay.Column);
            preMea ->NxtMea = tMeasure;
            tMeasure ->timeStamp = tChart[3];
            /*tMeasure ->Bar = new char [NowPlay.Column];
            tMeasure ->NxtMea = nullptr;
            tMeasure ->timeTable = nullptr;
            tMeasure ->ifHold = false;*/
        }

        //printf("%5d %5d %5d with column:", tChart[0], tChart[1], tChart[2]);
        ///< @brief for testing it can work well

        t = getKeyWords(chartFile, "endbeat", "column");
        if (!t) throw  ChartError(5);

        if (t == 1){
            ///< @brief to deal the hold key

            for (short i = 4; i < 7; ++i) getValueInt(chartFile, tChart[i]);
            tChart[7] = BeatToTime(NowPlay.everyBeat, tChart[4], tChart[5], tChart[6]);
            // tChart[7] = (int )(everyBeat * (float )(tChart[4] - 1) + everyBeat / (float )tChart[6] * (float )tChart[5] + 0.5f);

            if ( !tMeasure->ifHold ){
                ///< @brief if there isn't hold, then create

                tMeasure->ifHold = true;
                tMeasure->timeTable = new int [NowPlay.Column];
                memset(tMeasure->timeTable, 0, sizeof(int )*NowPlay.Column);
            }
            if ( !getKeyWord(chartFile, "column") ) throw ChartError(5);

            getValueInt(chartFile, tChart[8]);
            if ( tChart[8] > NowPlay.Column-1 ){
                NowPlay.ChartHead = nullptr;
                preMea = nullptr;
                ptrMea = (void* )new char[sizeof(temple)];
                tMeasure = new(ptrMea) Chart :: Measure(NowPlay.Column);
                continue ;
            }
            tMeasure->timeTable[ tChart[8] ] = tChart[7];
            NowPlay.noteCount += (int )(tMeasure->Bar[ tChart[8] ] = 2);
        }

        else {
            ///< @brief to deal the normal key

            getValueInt(chartFile, tChart[8]);
            if ( tChart[8] > NowPlay.Column-1 ){
                NowPlay.ChartHead = nullptr;
                preMea = nullptr;
                ptrMea = (void* )new char[sizeof(temple)];
                tMeasure = new(ptrMea) Chart :: Measure(NowPlay.Column);
                /* tMeasure ->timeStamp = 0;
                tMeasure ->Bar = new char [NowPlay.Column];
                tMeasure ->NxtMea = nullptr;
                tMeasure ->timeTable = nullptr;
                tMeasure ->ifHold = false;*/
                continue ;
            }
            tMeasure->Bar[ tChart[8] ] = 1;
            ++NowPlay.noteCount;
        }
    }

    /** deal other data */
    fseek(chartFile, 0, SEEK_SET);
    if (getKeyWord(chartFile, "offset")) getValueInt(chartFile, NowPlay.Offset);
    NowPlay.accPerNote = 100.0l/(NowPlay.noteCount);

    /** end the list */
    delete[] tChart;
    tMeasure ->NxtMea = nullptr;
    fclose(chartFile);
}

/**
 * @brief　to get chart meta data
 *
 * @param chartFile
 * @param songTitle
 * @param Artist
 ***/
void getChartMeta(FILE *chartFile, wchar_t *songTitle, wchar_t *Artist) {

    //打开谱面文件，我这段先写malody谱面的情况，osu谱面以后再考虑，因为两种的格式不大一样
    if (chartFile == nullptr) throw ChartError(0);
/*
 * 打算采用时间戳取代具体的节拍，即导入时算出时间戳
 */
    //获取谱面元数据
    /*
     * 该位置js代码如下（.mc文件中无换行与缩进，该代码为样例）
     * "meta": {
		    "$ver": 0,
		    "creator": "红色电音极地大冲击",
		    "background": "BG (2).jpg",
		    "version": "4K HI HandStream",
		    "id": 0,
		    "mode": 0,
		    "time": 1702658284,
		    "song": {
		    	"title": "我想对你说Baby(Yu vs. CTM Radio Mix)",
			    "artist": "VAVA",
		    	"id": 0
		    },
		    "mode_ext": {
			    "column": 4,
			    "bar_begin": 0
		    }
	    },
     */
    if (!getKeyWord(chartFile, "meta")) throw ChartError(1);
    if (!getKeyWord(chartFile, "song")) throw ChartError(1);
    if (!getKeyWord(chartFile, "title")) throw ChartError(1);
    getWords_w(chartFile, songTitle);
    if (!getKeyWord(chartFile, "artist")) throw ChartError(1);
    getWords_w(chartFile, Artist);
}

void changeJudgment(const int *NewJudge){
    int ptr = 0;
    if ( !(*NewJudge) ) throw ChartError(6);
    else *MaxOffset = *NewJudge;
    while (NewJudge[++ptr]){
        if (NewJudge[ptr] < NewJudge[ptr-1]) throw ChartError(6);
        MaxOffset[ptr] = NewJudge[ptr];
    }
    while (ptr < 8) MaxOffset[ptr++] = InfOffset;
}
