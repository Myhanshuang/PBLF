//
// Created by Logib on 2024/12/17.
//

#ifndef KEYRHYTHM_CHART_H
#include "Chart.h"
#include "User.h"

#endif


/**
 * @class User
 * @brief initialize user data
 */
User :: User(){
    memset(this ->userName, 0, sizeof(char )*13);
    memset(this ->userName, 0, sizeof(char )*21);
}

/**
 * @class User
 * @brief to save user data
 * @param Saving (\c FILE )
 */
void User :: save(FILE *Saving){
    if (this ->userName[0] == '\0' || this ->userName[12] != '\0')
        throw ChartError(7);
    if (this ->Password[0] == '\0' || this ->Password[20] != '\0')
        throw ChartError(8);
    fputc((int )'\n', Saving);
    fputs(this ->userName, Saving);
    fputc((int )'\n', Saving);
    char *encode = base64_encode(this ->Password);
    fputs(encode, Saving);
    fputc((int )'\n', Saving);
    free(encode);
}

/**
 * @class User
 * @brief to check if this is new user
 * @param Saving (\c FILE )
 * @return \c bool (1 = new, 0 = old)
 */
bool User :: newUser(FILE *Saving) {
    fseek(Saving, 0, SEEK_SET);
    return !( getKeyWord(Saving, this ->userName) );
}

/**
 * @class User
 * @brief to check whether the user's password is right
 * @param Saving (\c FILE )
 * @return \c bool (1 = right, 0 = wrong)
 */
bool User :: correct(FILE *Saving) {
    fseek(Saving, 0, SEEK_SET);
    if ( !getKeyWord(Saving, this ->userName) ) throw ChartError(9);
    char *tmp = new char [257];
    memset(tmp, 0, sizeof (char )*257);
    int t = 0;
    char ch = (char )getc(Saving);
    while (ch != '=' && !(ch > '0'-1 && ch < '9'+1)
           && !(ch > 'A'-1 && ch < 'Z'+1) && !(ch > 'a'-1 && ch < 'z'+1))
        ch = (char )getc(Saving);

    while (ch == '=' || (ch > '0'-1 && ch < '9'+1)
           || (ch > 'A'-1 && ch < 'Z'+1) || (ch > 'a'-1 && ch < 'z'+1)){
        tmp[t++] = ch;
        ch = (char )getc(Saving);
    }
    tmp[t]='\0';
    char *decode = base64_decode(tmp);
    delete[] tmp;
    if ( strcmp(decode, this ->Password) != 0){
        free(decode);
        return false;
    }
    free(decode);
    return true;
}

/**
 * @class UserSaveData
 * @brief constructor of UserSaveData
 */
UserSaveData :: UserSaveData() {
    this ->playData = Chart :: ChartAct();
    memset(this ->userName, 0, sizeof(char )*13);
    this->timeSign = 0;
    wcsset(this ->songTitle, L'\0');
}

/**
 * @class UserSaveData
 * @brief initialize UserSaveData with User U
 * @param U
 */
UserSaveData::UserSaveData(User U) {
    this ->playData = Chart :: ChartAct();
    memcpy(this ->userName, U.userName, strlen(U.userName));
    this ->userName[strlen(U.userName)] = '\0';
    this->timeSign = 0;
    wcsset(this ->songTitle, L'\0');
}

/**
 * @class UserSaveData
 * @brief initialize UserSaveData with some info, then can be saved
 * @param U
 * @param CA
 * @param title
 */
UserSaveData :: UserSaveData(User U, Chart::ChartAct &CA, wchar_t *title) {
    memcpy(this ->userName, U.userName, sizeof(U.userName));
    wcscpy(this ->songTitle, title);
    this ->playData = CA;
    this->timeSign = time(nullptr);
}

/**
 * @class UserSaveData
 * @brief destructor of UserSaveData
 */
UserSaveData :: ~UserSaveData() {
    this ->playData = Chart :: ChartAct();
    memset(this ->userName, 0, sizeof(char )*13);
    wcsset(this ->songTitle, L'\0');
    this->timeSign = 0;
}

/** BUG UNDER but not very serious */
/** bug fixed easily, it happened because I set too much char[], then mistook */

/**
 * @class UserSaveData
 * @brief put the path of history, then add the play record into it
 * @param historyPath
 */
void UserSaveData :: save(const char *historyPath) {
    DIR *ptr = opendir(historyPath);
    if (ptr == nullptr) throw ChartError(10);
    char *base = new char [strlen(historyPath)+39];
    memset(base, 0, sizeof (char )*(strlen(historyPath)+39));
    memcpy(base, historyPath, strlen(historyPath));
    strcat(base, "/");
    strcat(base, this ->userName);

    ptr = opendir(base);
    if (ptr == nullptr){
#ifdef WIN32
        mkdir(base);
#endif
#ifdef linux
        _mkdir(base, 0777);
#endif
    }
    ptr = opendir(base);
    if (ptr == nullptr) throw ChartError(10);
    strcat(base, "/");
    struct dirent *now = readdir(ptr);
    char *path = new char [strlen(base)+59];
    memset(path, 0, sizeof (char )*(strlen(base)+59));
    memcpy(path, base, sizeof (char )*(strlen(base)));
    FILE *fp = nullptr;
    time_t last;
    short t;
    char *st = new char [29];
    char *se = new char [29];
    {
        memset(st, 0, sizeof (char )*29);
        memset(se, 0, sizeof (char )*29);
        sprintf(st, "%lld", this ->timeSign);
        t = static_cast <short> (strlen(st)+2);
        memset(st, 0, sizeof (char )*29);
    }
    while (strcmp(now->d_name, "..")!=0) now = readdir(ptr);
    now = readdir(ptr);
    while (now != nullptr){
        strcat(path, now->d_name);
        fp = fopen(path, "r+");

        if (fp == nullptr) throw ChartError(10);
        if (getKeyWord_w(fp, this ->songTitle)){
            fseek(fp, -t, SEEK_END);
            while (!feof(fp)) getValueTT(fp, last);
            sprintf(st, "%lld", last);
            sprintf(se,"%s", now->d_name);
            //To search according to index(timestamp) to find the history record of the last play
            while (strcmp(st, se) != 0){
                sprintf(se, "%s", st);
                fclose(fp);
                path[strlen(base)] = '\0';
                strcat(path, st);
                fp = fopen(path, "r+");
                fseek(fp, -t, SEEK_END);
                while (!feof(fp)) getValueTT(fp, last);
                sprintf(st, "%lld", last);
            }

            //To modify the history record and add new record
            fseek(fp, -t, SEEK_END);
            sprintf(st, "%lld", this ->timeSign);
            fputs(st, fp);
            fclose(fp);
            path[strlen(base)] = '\0';
            strcat(path, st);
            fp = fopen(path, "w+");
            fputws(this ->songTitle, fp);
            fputwc(L'\n', fp);
            char *ss = new char [87];
            memset(ss, 0, sizeof (char )*87);
            sprintf(ss, "%d %.4Lf\n%d\n%d %d %d %d %d %d\n%s\n",
                    this ->playData.Score,
                    this ->playData.Accuracy,
                    this ->playData.maxCombo,
                    this ->playData.judgeResult[0],
                    this ->playData.judgeResult[1],
                    this ->playData.judgeResult[2],
                    this ->playData.judgeResult[3],
                    this ->playData.judgeResult[4],
                    this ->playData.judgeResult[5],
                    st);
            fputs(ss, fp);
            {
                delete[] ss;
                delete[] se;
                delete[] st;
                fclose(fp);
                closedir(ptr);
                delete[] path;
                delete[] base;
            }
            return ;
        }
        fclose(fp);
        path[strlen(base)] = '\0';
        now = readdir(ptr);
    }

    //If no play record was found, then create a new
    sprintf(st, "%lld", this ->timeSign);
    path[strlen(base)] = '\0';
    path[strlen(base) + 1] = '\0';
    strcat(path, st);
    fp = fopen(path, "w+");
    fputws(this ->songTitle, fp);
    fputwc(L'\n', fp);
    char *ss = new char [87];
    memset(ss, 0, sizeof (char )*87);
    sprintf(ss, "%d %.4Lf\n%d\n%d %d %d %d %d %d\n%s\n",
            this ->playData.Score,
            this ->playData.Accuracy,
            this ->playData.maxCombo,
            this ->playData.judgeResult[0],
            this ->playData.judgeResult[1],
            this ->playData.judgeResult[2],
            this ->playData.judgeResult[3],
            this ->playData.judgeResult[4],
            this ->playData.judgeResult[5],
            st);
    fputs(ss, fp);
    {
        delete[] ss;
        delete[] se;
        delete[] st;
        fclose(fp);
        closedir(ptr);
        delete[] path;
        delete[] base;
    }
}

/**
 * @class UserSaveData
 * @brief remove history record which named for timestamp from history path
 * @param historyPath
 * @param timeStamp
 * @return \c bool (0 = done, 1 = failed)
 */
bool UserSaveData :: removeData(const char *historyPath, time_t timeStamp){
    DIR *ptr = opendir(historyPath);
    if (ptr == nullptr) throw ChartError(10);
    char *base = new char [strlen(historyPath)+39];
    memset(base, 0, sizeof (char )*(strlen(historyPath)+39));
    memcpy(base, historyPath, strlen(historyPath));
    strcat(base, "/");
    strcat(base, this ->userName);
    ptr = opendir(base);
    if (ptr == nullptr) throw ChartError(10);
    strcat(base, "/");

    struct dirent *now = readdir(ptr);
    char *path = new char [strlen(base)+59];
    memset(path, 0, sizeof (char )*(strlen(base)+59));
    memcpy(path, base, sizeof (char )*(strlen(base)));
    FILE *fp = nullptr;
    time_t last, cur;
    short t;
    char *st = new char [29];
    char *se = new char [29];
    {
        memset(st, 0, sizeof (char )*29);
        memset(se, 0, sizeof (char )*29);
        sprintf(st, "%lld", timeStamp);
        t = static_cast <short> (strlen(st)+2);
        strcat(path, st);
        FILE *del = fopen(path, "r");
        if (del == nullptr) throw ChartError(14);

        getWordsW(del, this ->songTitle);
        //fgetws(this ->songTitle, sizeof (this ->songTitle)/ sizeof (wchar_t ), del);
        fseek(del, -t, SEEK_END);
        while (!feof(del)) getValueTT(del, cur);
        fclose(del);
        remove(path);
        path[strlen(base)] = '\0';
        memset(st, 0, sizeof (char )*29);
    }
    while (strcmp(now->d_name, "..")!=0) now = readdir(ptr);
    now = readdir(ptr);
    while (now != nullptr) {
        strcat(path, now->d_name);
        fp = fopen(path, "r+");

        if (fp == nullptr) throw ChartError(10);
        if (getKeyWord_w(fp, this->songTitle)) {
            fseek(fp, -t, SEEK_END);
            while (!feof(fp)) getValueTT(fp, last);
            sprintf(st, "%lld", last);
            sprintf(se, "%s", now->d_name);
            //To search according to index(timestamp) to find the history record of the last play before the removed one
            while (last != timeStamp) {
                sprintf(se, "%s", st);
                fclose(fp);
                path[strlen(base)] = '\0';
                strcat(path, st);
                fp = fopen(path, "r+");
                fseek(fp, -t, SEEK_END);
                while (!feof(fp)) getValueTT(fp, last);
                sprintf(st, "%lld", last);
            }
            //se is now's name, st is now's next

            //To modify the history record
            fseek(fp, -t, SEEK_END);

            if (timeStamp != cur) sprintf(se, "%lld", cur);
            fputs(se, fp);

            fclose(fp);
            {
                delete[] se;
                delete[] st;
                fclose(fp);
                closedir(ptr);
                delete[] path;
                delete[] base;
            }
            return false;
        }
        fclose(fp);
        path[strlen(base)] = '\0';
        now = readdir(ptr);
    }
    delete[] se;
    delete[] st;
    fclose(fp);
    closedir(ptr);
    delete[] path;
    delete[] base;
    return true;
}

/**
 * @class UserSaveData
 * @brief an unsafe func to get UserSaveData from history file
 * @details it read data except for timeSign (timestamp, used as file name)
 * \n and it doesn't check if pointer his is nullptr, so it should be checked before
 * @param his
 */
void UserSaveData :: getFromFile(FILE *his) {
    getWordsW(his, this ->songTitle);
    getValueInt(his, this ->playData.Score);
    getValueLDouble(his, this ->playData.Accuracy);
    getValueInt(his, this ->playData.maxCombo);
    for (int i = 0; i < 6; ++i) getValueInt(his, this ->playData.judgeResult[i]);
}

/**
 * @class UserSaveData
 * @brief get history record which named for timestamp from history path
 * @param historyPath
 * @param timeStamp
 * @return \c bool (0 = done, 1 = failed)
 */
bool UserSaveData :: getData(const char *historyPath, time_t timeStamp) {
    if (this ->userName[0] == '\0') throw ChartError(15);

    DIR *ptr = opendir(historyPath);
    if (ptr == nullptr) throw ChartError(10);
    char *base = new char [strlen(historyPath)+59];
    memset(base, 0, sizeof (char )*(strlen(historyPath)+59));
    memcpy(base, historyPath, strlen(historyPath));
    strcat(base, "/");
    strcat(base, this ->userName);
    strcat(base, "/");
    char *st = new char [20];
    sprintf(st, "%lld", timeStamp);
    strcat(base, st);
    delete[] st;
    FILE *his = fopen(base, "r");
    if (his == nullptr){
        closedir(ptr);
        delete[] base;
        return true;
    }

    this ->getFromFile(his);
    this ->timeSign = timeStamp;

    fclose(his);
    closedir(ptr);
    delete[] base;
    return false;
}

/**
 * @class UserSaveData
 * @brief get history record from history path, in chronological order
 * @details this func use inner counter, read the dir until cnt equals to count,
 * \n so if it's need to use it, you need push a outer counter to make it feasible,
 * \n and if outer counter, "count", is beyond the quantity of history records,
 * \n it will return 1, which means a failure of getting history record
 * @param historyPath
 * @param count
 * @return \c bool (0 = done, 1 = failed)
 */
bool UserSaveData :: getAnyData(const char *historyPath, int count) {
    if (this ->userName[0] == '\0') throw ChartError(15);

    DIR *ptr = opendir(historyPath);
    if (ptr == nullptr) throw ChartError(10);
    char *base = new char [strlen(historyPath)+59];
    memset(base, 0, sizeof (char )*(strlen(historyPath)+59));
    memcpy(base, historyPath, strlen(historyPath));
    strcat(base, "/");
    strcat(base, this ->userName);
    ptr = opendir(base);
    if (ptr == nullptr) throw ChartError(10);
    strcat(base, "/");

    struct dirent *now = readdir(ptr);
    FILE *fp = nullptr;
    int cnt = 0;

    while (strcmp(now->d_name, "..")!=0) now = readdir(ptr);
    now = readdir(ptr);

    while (now != nullptr) {
        if (++cnt != count){
            now = readdir(ptr);
            continue;
        }
        strcat(base, now->d_name);
        fp = fopen(base, "r+");
        if (fp == nullptr) throw ChartError(10);
        this ->getFromFile(fp);
        char *st;
        this ->timeSign = strtoll(now->d_name, &st, 10);

        fclose(fp);
        closedir(ptr);
        delete[] base;
        return false;

    }

    closedir(ptr);
    delete[] base;
    return true;
}

/**
 * @class UserSaveData
 * @brief get history record from history path, under the index of song title
 * @details this func return a time_t which point to the next piece of history record
 * of this song, \n and when it equals to this->timeSign, then it is the newest
 * \n the timeSign will change after an operation
 * @param historyPath
 * @param ch song title
 * @return \c time_t (general value = done, -1 = failed)
 */
time_t UserSaveData :: getSongData(const char *historyPath, wchar_t *ch) {
    this ->timeSign = 0;
    DIR *ptr = opendir(historyPath);
    if (ptr == nullptr) throw ChartError(10);
    char *base = new char [strlen(historyPath)+39];
    memset(base, 0, sizeof (char )*(strlen(historyPath)+39));
    memcpy(base, historyPath, strlen(historyPath));
    strcat(base, "/");
    strcat(base, this ->userName);
    ptr = opendir(base);
    if (ptr == nullptr) throw ChartError(10);
    strcat(base, "/");

    struct dirent *now = readdir(ptr);
    char *path = new char [strlen(base)+59];
    memset(path, 0, sizeof (char )*(strlen(base)+59));
    memcpy(path, base, sizeof (char )*(strlen(base)));
    FILE *fp = nullptr;
    time_t last, cur;
    short t;
    char *st = new char [29];
    char *se = new char [29];

    {
        memset(st, 0, sizeof (char )*29);
        memset(se, 0, sizeof (char )*29);
        sprintf(st, "%lld", this ->timeSign);
        t = static_cast <short> (strlen(st)+2);
        memset(st, 0, sizeof (char )*29);
    }
    while (strcmp(now->d_name, "..")!=0) now = readdir(ptr);
    now = readdir(ptr);
    while (now != nullptr){
        strcat(path, now->d_name);
        fp = fopen(path, "r+");

        if (fp == nullptr) throw ChartError(10);
        if (getKeyWord_w(fp, this ->songTitle)){
            fseek(fp, -t, SEEK_END);
            while (!feof(fp)) getValueTT(fp, last);

            if (this ->timeSign == 0){
                fseek(fp, 0, SEEK_SET);
                this ->getFromFile(fp);
                char *c;
                this ->timeSign = strtoll(now->d_name, &c, 10);
                {
                    delete[] se;
                    delete[] st;
                    fclose(fp);
                    closedir(ptr);
                    delete[] path;
                    delete[] base;
                }

                return last;
            }

            sprintf(st, "%lld", last);
            sprintf(se,"%s", now->d_name);
            //To search according to index(timestamp) to find the history record of the last play
            while (this ->timeSign != last){
                sprintf(se, "%s", st);
                fclose(fp);
                path[strlen(base)] = '\0';
                strcat(path, st);
                fp = fopen(path, "r+");
                fseek(fp, -t, SEEK_END);
                while (!feof(fp)) getValueTT(fp, last);
                sprintf(st, "%lld", last);
            }
            sprintf(se, "%s", st);
            fclose(fp);
            path[strlen(base)] = '\0';
            strcat(path, st);
            fp = fopen(path, "r+");
            fseek(fp, -t, SEEK_END);
            while (!feof(fp)) getValueTT(fp, last);

            //To get the history record and return the next time_t
            fseek(fp, 0, SEEK_SET);
            this ->getFromFile(fp);
            char *c;
             this ->timeSign = strtoll(se, &c, 10);

            {
                delete[] se;
                delete[] st;
                fclose(fp);
                closedir(ptr);
                delete[] path;
                delete[] base;
            }
            return last;
        }
        fclose(fp);
        path[strlen(base)] = '\0';
        now = readdir(ptr);
    }
    return -1;
}