#pragma once

#include <cstdint>

#include "game_state.hh"

class TGameSolver {
    static bool IsAbleToAction(const TGameState &state, size_t from, size_t to) {
        return !state.GetStack(from).IsEmpty() && state.GetStack(to).AbleToAdd(state.GetStack(from).GetTopCardView());
    }

    static TGameState Action(const TGameState &state, size_t from, size_t to);

    static std::set<TGameState> PossibleStates(const TGameState &state);

public:
    static ssize_t Solve(const TGameState &startState);
};
