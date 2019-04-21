#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include "DB.h"
#include "Callback.h"

using namespace Database;
using namespace Callback;
using namespace std;

DB::DB() {
    zErrMsg = 0;
    data = "Callback function called";
}

bool DB::init() {
    /* Open database */
   rc = sqlite3_open("tetris.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return false;
   } else {
      fprintf(stderr, "Opened database successfully\n");
      return true;
   }
}

string DB::insert(string n, string s, string a) {

   /* Create SQL statement */
   sql = "INSERT INTO leaderboard (name, score, added) "  \
         "VALUES('" + n + "', '" + s + "', '" + a + "'); ";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), insertion, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      return "SQL error.";
   } else {
      fprintf(stdout, "Records created successfully\n");
      return "Records created successfully\n";
   }
   
};

void DB::select(vector<LeaderboardRecord> &res) {

    /* Create SQL statement */
   sql = "SELECT * from leaderboard";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), selection, &res, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }

}

DB::~DB() {
   
   sqlite3_close(db);

}