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
            int rc;
            string sql;
            const char* data;
            // int callback(void *data, int argc, char **argv, char **azColName);
        public:
            DB();
            ~DB();
            string insert(string n, string s, string a);
            void select(vector<LeaderboardRecord> &res);
            bool init();

    };

}

#endif