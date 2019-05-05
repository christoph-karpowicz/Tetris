#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <vector>
#include <string>

using namespace std;

namespace Database {

    struct LeaderboardRecord {
        string name;
        string score;
        string added;
    };

    class DB {
        private:
            sqlite3 *db;
            char *zErrMsg = 0;
            int conn;
            string sql;
            const char* db_name;
            // int callback(void *data, int argc, char **argv, char **azColName);
        public:
            DB();
            bool init();
            void select(vector<LeaderboardRecord> &res);
            string insert(string const n, string const s, string const a);
            ~DB();

    };

}

#endif