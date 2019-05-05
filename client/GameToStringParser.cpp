#include "objects.h"

using namespace G;

GameToStringParser GameToStringParser::parser;

std::string GameToStringParser::parse(Game &game) {
    list<Square*> squaresStill = game.getSquaresList();
    std::string returnString = "";

    returnString.append("SquaresStill: ");
    for (Square* s : squaresStill) {
        returnString.append("{ x: " + std::to_string(s->getX()) + ", y: " + std::to_string(s->getY()) + " } ");
    }

    returnString.append("SquaresInSquareSet: ");
    SquareSet* activeSquareSet = game.getActiveSquareSet();
    Square*** squaresInSquareSet = activeSquareSet->getSquares();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squaresInSquareSet[i][j] != 0) {
                returnString.append("{ x: " + std::to_string(squaresInSquareSet[i][j]->getX()) + ", y: " + std::to_string(squaresInSquareSet[i][j]->getY()) + " } ");
            }
        }
    }

    returnString.append("GameSpeed: " + std::to_string(game.getGameSpeed()) + " ");
    returnString.append("GameScore: " + std::to_string(game.getScore()) + " ");

    return returnString;
};

std::string GameToStringParser::parseGame(Game* game) {
    return parser.parse(*game);
};