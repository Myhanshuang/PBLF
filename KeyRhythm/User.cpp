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
    this->timeSign = time(nullptr);
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
    this ->songTitle = new wchar_t[wcslen(title)+2];
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
    delete[] this ->songTitle;
    this ->songTitle = nullptr;
    this->timeSign = 0;
}


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
    int t;/*
#ifdef WIN32
    t = chdir(base);
#endif
#ifdef linux
    t = _chdir(base);
#endif
    if (t == -1) throw ChartError(10);*/
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

        if (fp == nullptr) throw ChartError(10);//error
        printf("%s\n", base);
        if (getKeyWord_w(fp, this ->songTitle)){
            fseek(fp, -(t+5), SEEK_END);
            while (!feof(fp)) getValueTT(fp, last);
            sprintf(st, "%lld", last);
            sprintf(se,"%s", now->d_name);
            printf("%s\n", base);
            //To search according to index(timestamp) to find the history record of the last play
            while (strcmp(st, se) != 0){
                sprintf(se, "%s", st);
                fclose(fp);
                memset(path + strlen(base), 0, strlen(se));
                path[strlen(historyPath)] = '\0';
                path[strlen(historyPath) + 1] = '\0';
                strcat(path, st);
                fp = fopen(path, "r+");
                fseek(fp, -t-1, SEEK_END);
                while (!feof(fp)) getValueTT(fp, last);
                sprintf(st, "%lld", last);
            }

            //To modify the history record and add new record
            fseek(fp, -t, SEEK_END);
            sprintf(st, "%lld", this ->timeSign);
            fputs(st, fp);
            fclose(fp);
            memset(path + strlen(base), 0, strlen(se));
            path[strlen(historyPath)] = '\0';
            path[strlen(historyPath) + 1] = '\0';
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
                base[strlen(base)-1-strlen(this ->userName)-1-7-1]='\0';
#ifdef WIN32
                t = chdir(base);
#endif
#ifdef linux
                t = _chdir(base);
#endif
                if (t == -1) throw ChartError(10);
                delete[] path;
                delete[] base;
            }
            return ;
        }
        fclose(fp);
        memset(path + strlen(base), 0, strlen(now->d_name));
        path[strlen(historyPath)] = '\0';
        path[strlen(historyPath) + 1] = '\0';
        now = readdir(ptr);
    }

    //If no play record was found, then create a new
    sprintf(st, "%lld", this ->timeSign);
    path[strlen(base)] = '\0';
    path[strlen(base) + 1] = '\0';
    strcat(path, st);
    fp = fopen(path, "w+");
    wprintf(L"%s\n", this ->songTitle);
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