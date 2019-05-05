#include <stdio.h>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "DB.h"
#include "Callback.h"

using namespace Callback;
using namespace Database;
using namespace std;

int Callback::selection(void *data, int argc, char **argv, char **azColName) {
    vector<LeaderboardRecord>* res = static_cast<vector<LeaderboardRecord>*>(data);
   
    LeaderboardRecord lr;
    for (int i = 0; i < argc; i++) {
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        const string col = azColName[i];
        switch (columns.at(col)) {
            case columnNames::name:
                lr.name = argv[i] ? argv[i] : "NULL";
                break;
            case columnNames::score:
                lr.score = argv[i] ? argv[i] : "0";
                break;
            case columnNames::added:
                lr.added = argv[i] ? argv[i] : "0";
                break;

        }
    }
    res->push_back(lr);
    
    return 0;
};

int Callback::insertion(void *data, int argc, char **argv, char **azColName) {
    // int i;
//    for(i = 0; i<argc; i++) {
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
   return 0;
};