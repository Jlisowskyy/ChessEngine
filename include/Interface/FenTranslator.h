//
// Created by Jlisowskyy on 12/27/23.
//

#ifndef FENTRANSLATOR_H
#define FENTRANSLATOR_H

#include <string>
#include <format>

#include "../EngineTypeDefs.h"

struct FenTranslator {
    // ------------------------------
    // Class interaction
    // ------------------------------

    static Board GetDefault() { return StartBoard; }
    static Board Translate(const std::string& fenPos)
        // Function simply translates position from FEN notation into inner representation.
    ;

    // ------------------------------
    // private methods
    // ------------------------------

    static size_t _processElPassant(Board& bd, size_t pos, const std::string& fenPos)
        // Function reads from fenPos ElPassant field specifying substring
        // and saves this field inside inner board representation.
        // Returns index of first blank character after that substring or EndOfString.
    ;

    static size_t _processCastlings(Board& bd, size_t pos, const std::string& fenPos)
        // Function reads from fenPos castling specifying substring and applies possibilites accordingly to that string.
        // Returns index of first blank character after that substring or EndOfString.
    ;

    static size_t _processMovingColor(Board& bd, size_t pos, const std::string& fenPos)
        // Function validates and applies moving color from fen notation into inner representation.
        // Returns first blank character after color specifying character, that is pos + 1.
    ;

    static size_t _processPositions(Board& bd, size_t pos, const std::string& fenPos)
        // Function translates fen figure representation to inner board representation.
        // Returns index of first blank character after the solid position substring or EndOfString.
    ;

    static void _addFigure(const std::string& pos, char fig, Board& bd)
        // Function simply adds figure encoded in 'fig' to board using map translating
        // character encoding to to actual figure represetntation. String 'pos' contains position
        // encoded in string also used to retreive inner board representation using translating map.
    ;

    static size_t _skipBlanks(size_t pos, const std::string& fenPos)
        // Function returns first non blank character inside fenPos substring,
        // which starts at 'pos' index and ends naturally
    ;

    // ------------------------------
    // class fields
    // ------------------------------

    inline static constexpr Board StartBoard = {
        .Castlings = { true, true, true, true },
        .elPassantField = INVALID,
        .movColor = WHITE,
        .boards = {
            65280LLU,
            66LLU,
            36LLU,
            129LLU,
            8LLU,
            16LLU,
            71776119061217280LLU,
            4755801206503243776LLU,
            2594073385365405696LLU,
            9295429630892703744LLU,
            576460752303423488LLU,
            1152921504606846976LLU,
        }
    };
};

#endif //FENTRANSLATOR_H
