#ifndef CALLBACK_H
#define CALLBACK_H

#include <sqlite3.h>
#include <map>

namespace Callback {

    enum class columnNames {
        name,
        score,
        added
    };
    static const std::map<std::string, columnNames> columns =
    {
        { "name", columnNames::name },
        { "score", columnNames::score },
        { "added", columnNames::added }
    };

    int insertion(void *data, int argc, char **argv, char **azColName);
    int selection(void *data, int argc, char **argv, char **azColName);

}

#endif