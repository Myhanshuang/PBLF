//
// Created by Logib on 2024/12/10.
//

#include "ChartAnalyzer.h"

/**
 * @brief use KMP to get the position of the keyword
 * @param File
 * @param Length
 * @param KeyWord
 * @return \c bool ( 1 = found )
 ***/
bool getKeyWord(FILE *File, const int Length, const char *KeyWord){
    char c = '\0';
    auto nxt = new short [Length+1];
    nxt[0] = -1;
    for (short i = 0, j = -1; i < Length-1; ){
        if (j == -1 || KeyWord[i] == KeyWord[j]){
            ++i;
            ++j;
            nxt[i] = j;
        }
        else j = nxt[j];
    }
    short j = -1;
    while (!feof(File)){
        if (j == -1 || c == KeyWord[j]){
            c = (char )fgetc(File);
            ++j;
        }
        else j = nxt[j];
        if (j == Length){
            delete[] nxt;
            return true;
        }
    }
    delete[] nxt;
    return false;
}

/**
 * @brief use multiple KMP to get the position of the keywords
 * @param File
 * @param Length1
 * @param Length2
 * @param KeyWord1
 * @param KeyWord2
 * @return \c short ( 1 = found 1, 2 = found 2, 0 = all not found)
 ***/
short getKeyWords(FILE *File, const int Length1, const int Length2, const char *KeyWord1, const char *KeyWord2){
    char c = '\0';
    auto nxt1 = new short [Length1+1];
    auto nxt2 = new short [Length2+1];
    nxt1[0] = -1, nxt2[0] = -1;
    for (short i = 0, j = -1; i < Length1-1; ){
        if (j == -1 || KeyWord1[i] == KeyWord1[j]){
            ++i;
            ++j;
            nxt1[i] = j;
        }
        else j = nxt1[j];
    }
    for (short i = 0, j = -1; i < Length2-1; ){
        if (j == -1 || KeyWord2[i] == KeyWord2[j]){
            ++i;
            ++j;
            nxt2[i] = j;
        }
        else j = nxt2[j];
    }
    short j1 = -1, j2 = -1;
    bool f1, f2;
    while (!feof(File)){
        f1 = (j1 == -1 || c == KeyWord1[j1]);
        f2 = (j2 == -1 || c == KeyWord2[j2]);
        if (f1 && f2){
            c = (char )fgetc(File);
            ++j1;
            ++j2;
        }
        else {
            while (!f1){
                j1 = nxt1[j1];
                f1 = (j1 == -1 || c == KeyWord1[j1]);
            }
            while (!f2){
                j2 = nxt2[j2];
                f2 = (j2 == -1 || c == KeyWord2[j2]);
            }
        }
        if (j1 == Length1){
            delete[] nxt1;
            delete[] nxt2;
            return 1;
        }
        if (j2 == Length2){
            delete[] nxt1;
            delete[] nxt2;
            return 2;
        }
    }
    delete[] nxt1;
    delete[] nxt2;
    return 0;
}

/**
 * @brief　to get chart
 *
 * @param ChartFileName
 * @return Chart
 ***/
Chart getChart(const char *ChartFileName){
    short t = 0;

    FILE *chartFile = fopen(ChartFileName, "r");
    //打开谱面文件，我这段先写malody谱面的情况，osu谱面以后再考虑，因为两种的格式不大一样
    if (chartFile == nullptr) throw ChartError(0);
    t = 0;
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
    if ( !getKeyWord(chartFile, 4, "meta") ) throw ChartError(1);
    if ( !getKeyWord(chartFile, 8, "mode_ext") ) throw ChartError(2);
    if ( !getKeyWord(chartFile, 6, "column") ) throw ChartError(2);
    fscanf_s(chartFile, "%hd", &t);
    Chart NowPlay(t);
    //获取bpm，同时确认每小节的时长
    /*
     * 该位置js代码如下（.mc文件中无换行与缩进，该代码为样例）
     * "time": [{
		    "beat": [0, 0, 1],
		    "bpm": 175.0
	    }],
     */


    if ( !getKeyWord(chartFile, 4, "time") ) throw ChartError(3);
    if ( !fscanf_s(chartFile, "%*hd%*hd%hd", &t)) throw ChartError(3);
    NowPlay.EveryBeat = FloatMinute / (float )t;
    if ( !getKeyWord(chartFile, 3, "bpm") ) throw ChartError(3);
    fscanf_s(chartFile, "%f", &NowPlay.BeatsPerMinute);
    NowPlay.EveryBeat /= NowPlay.BeatsPerMinute;

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

    if ( !getKeyWord(chartFile, 4, "note") ) throw ChartError(4);

    /* int tBar,  tBeat, tDivision, tTimeStamp, tKey;
     * tChart[0]  [1]   [2]        [3]         [8]
     * endBar, endBeat, endDivision, endTimeStamp,
     * [4]     [5]      [6]          [7]
     */
    int *tChart = new int [9];
    const Chart :: Measure temple(NowPlay.Column);

    void *ptrMea = (void* )new char[sizeof(temple)];
    auto tMeasure = new(ptrMea) Chart :: Measure(NowPlay.Column);
    Chart :: Measure *preMea = nullptr;

    while (!feof(chartFile)){
        if ( !getKeyWord(chartFile, 4, "beat") ){
            if (!feof(chartFile)) throw ChartError(5);
            delete[] tChart;
            tMeasure ->NxtMea = nullptr;
            return NowPlay;
        }

        fscanf_s(chartFile, "%d%d%d", &tChart[0], &tChart[1], &tChart[2]);
        ///< @brief to get the beat info and turn it into timestamp
        tChart[3] = BeatToTime(NowPlay.EveryBeat, tChart[0], tChart[1], tChart[2]);
        // tChart[3] = (int )(EveryBeat * (float )(tChart[0] - 1) + EveryBeat / (float )tChart[2] * (float )tChart[1] + 0.5f);

        if (tMeasure->timeStamp != tChart[3]){
            if (NowPlay.ChartHead == nullptr) NowPlay.ChartHead = tMeasure;
            preMea = tMeasure;
            ptrMea = (void* )new char[sizeof(temple)];
            tMeasure = new(ptrMea) Chart :: Measure(NowPlay.Column);
            preMea ->NxtMea = tMeasure;
        }

        t = getKeyWords(chartFile, 7, 6, "endbeat", "column");
        if (!t) throw  ChartError(5);

        if (t == 1){
            ///< @brief to deal the hold key

            fscanf_s(chartFile, "%d%d%d", &tChart[4], &tChart[5], &tChart[6]);
            tChart[7] = BeatToTime(NowPlay.EveryBeat, tChart[4], tChart[5], tChart[6]);
            // tChart[7] = (int )(EveryBeat * (float )(tChart[4] - 1) + EveryBeat / (float )tChart[6] * (float )tChart[5] + 0.5f);

            if (!tMeasure->ifHold){
                tMeasure->ifHold = true;
                tMeasure->timeTable = new int [NowPlay.Column];
                memset(tMeasure->timeTable, 0, sizeof(int )*NowPlay.Column);
            }
            if (!getKeyWord(chartFile, 6, "column")) throw ChartError(5);

            fscanf_s(chartFile, "%d", &tChart[8]);
            tMeasure->timeTable[ tChart[8] ] = tChart[7];
            tMeasure->Bar[ tChart[8] ] = 2;
        }
        else {
            ///< @brief to deal the normal key
            fscanf_s(chartFile, "%d", &tChart[8]);
            tMeasure->Bar[ tChart[8] ] = 1;
        }
    }

    delete[] tChart;
    fclose(chartFile);
    return NowPlay;
}