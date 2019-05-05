#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>

#include <fstream>
#include <cstring>
#include <string>
#include <evhttp.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>
#include <tuple>
#include <cassert>
#include "Server.h"
#include "DB.h"

using namespace Database;
using namespace ServerN;

Router::Router(Database::DB *&db): database(db) {
    createGETResponses();
};

void Router::createGETResponses() const {

    // getLeaderboard request
    GETResponses["getLeaderboard"] = [](Database::DB* db, const std::map<std::string, std::string> &inputData) {
        vector<LeaderboardRecord> leaderboardDBRecords;
        leaderboardDBRecords = {};
        GETResponse res;
        
        string response = "{ ";
        response = response + " \"leaderboard\": [";
        db->select(leaderboardDBRecords);
        std::sort(leaderboardDBRecords.begin(), leaderboardDBRecords.end(), [](LeaderboardRecord a, LeaderboardRecord b) {
            return std::stoi(a.score) > std::stoi(b.score);
        });
        for (LeaderboardRecord lr : leaderboardDBRecords) {
            response = response + " { ";
            response = response + " \"name\": \"" + lr.name + "\", ";
            response = response + " \"score\": \"" + lr.score + "\", ";
            response = response + " \"added\": \"" + lr.added + "\"";
            // cout << "name: " << lr.name << " | score: " << lr.score << " | added: " << lr.added << endl;
            response = response + " }, ";
        }
        leaderboardDBRecords.clear();
        response = response + "\"e\" ] }";
        res.setResponse(response);
        return res;
    };

    // insertIntoLeaderboard request
    GETResponses["insertIntoLeaderboard"] = [](Database::DB* db, const std::map<std::string, std::string> &inputData) {
        GETResponse res;

        db->insert(inputData.at("name"), inputData.at("score"), inputData.at("added"));

        std::string response = "Record saved.";
        
        res.setResponse(response);
        return res;
    };

    // saveGame request
    GETResponses["saveGame"] = [](Database::DB* db, const std::map<std::string, std::string> &inputData) {
        GETResponse res;
        
        cout << inputData.at("gameString") << endl;
        
        std::ofstream file("server/saves/save.txt");
        file << inputData.at("gameString") << std::endl;
        file.close();

        std::string response = "Game saved.";
        
        res.setResponse(response);
        return res;
    };

};

char* Router::operator()(const std::string &getRequest, const std::map<std::string, std::string> &inputData) const {

    GETResponse res = GETResponses[getRequest](database, inputData);
    string resString = res.getResponse();
    char* resCharArray = new char[resString.length() + 1];
    std::strcpy (resCharArray, resString.c_str());

    return resCharArray;

};

// GETResponse nested class
std::string Router::GETResponse::getResponse() const {
    return response;
};

void Router::GETResponse::setResponse(const std::string res) {
    response = res;
};