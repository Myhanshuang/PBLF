#include <iostream>
#include <exception>
#include <cstdio>
#include <cstring>

#define Minute 60000.0f
/**
 * @brief trans beat to timestamp
 *
 * @details according to [a, b, c] in .mz file
 * @param eb everybeat
 * @param a which bar
 * @param b which beat/measure
 * @param c the division
 *
 * @return timestamp (int)
 */
#define BeatToTime(eb, a, b, c) (int )(eb * (float )(a - 1) + eb / (float )c * (float )b + 0.5f)

#define Exception0 "No chart file"
///< @brief cannot find the chart file
#define Exception1 "Lose meta data"
///< @brief cannot find metadata in chart file
#define Exception2 "Lose column info or bar begin info"
///< @brief cannot fine "mode_ext" in chart file
#define Exception3 "Lose BPM or unit beat of bar"
///< @brief cannot fine beat info in chart file
#define Exception4 "Lose note in chart"
///< @brief cannot fine note in chart file
#define Exception5 "Chart format error"
///< @brief cannot fine "column" or beat of note in chart file

/**
 * @brief The chart data
 *
 *
 * @details It included a head of metadata and several bar s
 * @param BPM (&Keys)
 ***/
class Chart;
class Chart{
public :
    float BeatsPerMinute = 0.0f, EveryBeat = 0.0f;
    int NoteCount = 0, Column = 0, Offset = 0;
    class Measure;
    Measure *ChartHead = nullptr;

    explicit Chart(const int Keys){
        this ->Column = Keys;
        this ->ChartHead = nullptr;
        //this ->CHS -= new [4];
    }
    Chart(){
        this ->Column = 4;
        this ->ChartHead = nullptr;
    }
    ~Chart();

};

/**
 * @brief The mini-est chart unit
 *
 * @details signed keys of every timestamp
 * about hold's end (if there is) and next Measure
 * it is a singly linked list
 * @param Keys
 */
class Chart :: Measure{
public :
    bool ifHold = false;
    int timeStamp = 0;
    int *timeTable = nullptr;
    char *Bar = nullptr;
    Measure *NxtMea = nullptr;

    Measure(){
        this ->timeStamp = 0;
        this ->timeTable = nullptr;
        this ->NxtMea = nullptr;
    }
    explicit Measure(const int Keys){
        this ->timeStamp = 0;
        this ->timeTable = nullptr;
        this ->NxtMea = nullptr;
        this ->Bar=new char [Keys];
    }
    ~Measure(){
        delete[] this ->Bar;
        delete[] this ->timeTable;
    }
    void* operator new(size_t size, void* phead){
        return phead;
    }

};

Chart :: ~Chart(){
    Chart :: Measure *temp = ChartHead, *preMea = nullptr;
    if (temp != nullptr){
        while(temp -> NxtMea != nullptr)
        {
            preMea = temp;
            temp = temp ->NxtMea;
            preMea ->~Measure();
            delete preMea;
            return ;
        }
        delete ChartHead;
    }
}

/**
 * @brief use KMP to get the position of the keyword
 * @param File
 * @param Length
 * @param KeyWord
 * @return bool ( 1 = found )
 */
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
 * @return short ( 1 = found 1, 2 = found 2, 0 = all not found)
 */
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
 *
 * @param ChartFileName
 * @return Chart
 */
Chart getChart(const char *ChartFileName){
    short t = 0;

    FILE *chartFile = fopen(ChartFileName, "r");
    //打开谱面文件，我这段先写malody谱面的情况，osu谱面以后再考虑，因为两种的格式不大一样
    if (chartFile == nullptr) throw Exception0;
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
    if ( !getKeyWord(chartFile, 4, "meta") ) throw Exception1;
    if ( !getKeyWord(chartFile, 8, "mode_ext") ) throw Exception2;
    if ( !getKeyWord(chartFile, 6, "column") ) throw Exception2;
    fscanf(chartFile, "%hd", &t);
    Chart NowPlay(t);
    //获取bpm，同时确认每小节的时长
    /*
     * 该位置js代码如下（.mc文件中无换行与缩进，该代码为样例）
     * "time": [{
		    "beat": [0, 0, 1],
		    "bpm": 175.0
	    }],
     */


    if ( !getKeyWord(chartFile, 4, "time") ) throw Exception3;
    if ( !fscanf(chartFile, "%*hd%*hd%hd", &t)) throw Exception3;
    NowPlay.EveryBeat = Minute / (float )t;
    if ( !getKeyWord(chartFile, 3, "bpm") ) throw Exception3;
    fscanf(chartFile, "%f", &NowPlay.BeatsPerMinute);
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

    if ( !getKeyWord(chartFile, 4, "note") ) throw Exception4;

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
            if (!feof(chartFile)) throw Exception5;
            delete[] tChart;
            tMeasure ->NxtMea = nullptr;
            return NowPlay;
        }

        fscanf(chartFile, "%d%d%d", &tChart[0], &tChart[1], &tChart[2]);
        ///< @brief to get the beat info and turn it into timestamp
        tChart[3] = BeatToTime(NowPlay.EveryBeat, tChart[0], tChart[1], tChart[2]);
        // tChart[3] = (int )(EveryBeat * (float )(tChart[0] - 1) + EveryBeat / (float )tChart[2] * (float )tChart[1] + 0.5f);

        if (tMeasure->timeStamp != tChart[3]){
            if (NowPlay.ChartHead == nullptr){
                NowPlay.ChartHead = tMeasure;
            }
            preMea = tMeasure;
            ptrMea = (void* )new char[sizeof(temple)];
            tMeasure = new(ptrMea) Chart :: Measure(NowPlay.Column);
            preMea ->NxtMea = tMeasure;
        }

        t = getKeyWords(chartFile, 7, 6, "endbeat", "column");
        if (!t) throw  Exception5;

        if (t == 1){
            ///< @brief to deal the hold key

            fscanf(chartFile, "%d%d%d", &tChart[4], &tChart[5], &tChart[6]);
            tChart[7] = BeatToTime(NowPlay.EveryBeat, tChart[4], tChart[5], tChart[6]);
            // tChart[7] = (int )(EveryBeat * (float )(tChart[4] - 1) + EveryBeat / (float )tChart[6] * (float )tChart[5] + 0.5f);

            if (!tMeasure->ifHold){
                tMeasure->ifHold = true;
                tMeasure->timeTable = new int [NowPlay.Column];
                memset(tMeasure->timeTable, 0, sizeof(int )*NowPlay.Column);
            }
            if (!getKeyWord(chartFile, 6, "column")) throw Exception5;

            fscanf(chartFile, "%d", &tChart[8]);
            tMeasure->timeTable[ tChart[8] ] = tChart[7];
            tMeasure->Bar[ tChart[8] ] = 2;
        }
        else {
            ///< @brief to deal the normal key
            fscanf(chartFile, "%d", &tChart[8]);
            tMeasure->Bar[ tChart[8] ] = 1;
        }
    }

    delete[] tChart;
    ptrMea = (char *) ptrMea;
    delete[] ptrMea;
    fclose(chartFile);
    return NowPlay;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    getChart("1.txt");
    return 0;
}
