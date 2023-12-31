//
// Created by Jlisowskyy on 12/30/23.
//

#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include <array>

#include "../BitOperations.h"
#include "../movesHashMap.h"
#include "../EngineTypeDefs.h"

[[nodiscard]] constexpr std::array<uint64_t, Board::BoardFields> GenStaticMoves(const int maxMovesCount,
    const int *movesCords, const int *rowCords)
{
    std::array<uint64_t, Board::BoardFields> movesRet{};

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            const int bInd = 8*y + x;
            const int msbInd = ConvertToReversedPos(bInd);

            uint64_t packedMoves = 0;
            for (size_t i = 0; i < maxMovesCount; ++i)  {
                const int moveYCord = (bInd + movesCords[i]) / 8;
                const int knightYCord = bInd / 8;

                if (const int moveInd = bInd + movesCords[i];
                    moveInd >= 0 && moveInd < 64 && moveYCord == knightYCord + rowCords[i])
                    packedMoves |= 1LLU << moveInd;
            }

            movesRet[msbInd] = packedMoves;
        }
    }

    return movesRet;
}

template<class neighborCountingFunc>
[[nodiscard]] constexpr size_t CalculateTotalOfPossibleHashMapElements(neighborCountingFunc func) {
    size_t sum{};
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            sum += func(x, y);
        }
    }

    return sum;
}

inline void DisplayMasks(const uint64_t* masks, const char** names, const size_t maskCount) {
    for (size_t i = 0; i < maskCount; ++i) {
        std::cout << "Mask name: " << names[i] << std::endl;
        DisplayMask(masks[i]);
    }
}

template<class comparisonMethod>
[[nodiscard]] constexpr uint64_t GenMask(const int barrier, int boardIndex, const int offset, comparisonMethod comp) {
    uint64_t mask = 0;

    while(comp(boardIndex += offset, barrier))
        mask |= (1LLU<<boardIndex);

    return mask;
}

[[nodiscard]] constexpr uint64_t GenMask(const int startInd, const int boarderIndex, const int offset) {
    uint64_t ret = 0;
    for (int i = startInd; i < boarderIndex; i += offset) ret |= (1LLU << i);
    return ret;
}

[[nodiscard]] constexpr size_t MyCeil(const double x) {
    return (static_cast<double>(static_cast<size_t>(x)) == x)
    ? static_cast<size_t>(x)
    : static_cast<size_t>(x) + ((x > 0) ? 1 : 0);
}

[[nodiscard]] constexpr size_t GetSize(const size_t x) {
    size_t size = 0;

    for (size_t i = 0; i < sizeof(size_t)*8; ++i) {
        if (const size_t mask = 1LLU << i; (mask & x) != 0)
            size = i;
    }

    return x == (1 << size) ? size : size + 1;
}

template<class sizeGenerator, class maskGenerator>
constexpr void MapInitializer(const uint64_t* aHash, const uint64_t* bHash,
    std::array<movesHashMap, Board::BoardFields>& maps, sizeGenerator sGen, maskGenerator mGen)
{
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const int bInd = y*8 + x;
            const int mapInd = ConvertToReversedPos(bInd);

            const size_t minimalSize = sGen(x, y);
            const size_t min2Pow = GetSize(minimalSize);
            const size_t mapSize = 1 << min2Pow; // 2^min2Pow
            const uint64_t moduloMask = mapSize - 1; // Mask all ones
            const uint64_t primeNumber = PrimeNumberMap[min2Pow];
            const uint64_t a = aHash[mapInd];
            const uint64_t b = bHash[mapInd];
            const auto masks = mGen(bInd);

            maps[mapInd] = movesHashMap{masks, a, b, moduloMask, primeNumber, mapSize};
        }
    }
}

template<class boundryCheckFunc>
constexpr uint64_t GenSlidingMoves(const uint64_t neighbors, const int bInd,
    const int offset, boundryCheckFunc boundryCheck)
{
    uint64_t ret = 0;
    int actPos = bInd;

    while (boundryCheck(actPos += offset)) {
        const uint64_t curMove = 1LLU << actPos;
        ret |= curMove;

        if ((curMove & neighbors) != 0) break;
    }

    return ret;
}

template<class moveGeneartor, class neighborGenerator>
constexpr void MoveInitializer(std::array<movesHashMap, Board::BoardFields>& maps, moveGeneartor mGen, neighborGenerator nGen) {
    for(int i = 0; i < Board::BoardFields; ++i) {
        const int bInd = ConvertToReversedPos(i);

        // Possible neighbors generation.
        const auto [possibilities, posSize] = nGen(bInd, maps[i]);

        for (size_t j = 0; j < posSize; ++j) {
            const uint64_t moves = mGen(possibilities[j], bInd);

            maps[i][possibilities[j]] = moves;
        }
    }
}

#endif //MOVEGENERATION_H
