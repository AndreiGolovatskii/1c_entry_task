#include <map>
#include <set>
#include <utility>

#include "game_solver.hh"


TGameState TGameSolver::Action(const TGameState &state, size_t from, size_t to) {
    TGameState res = state;
    auto slice = res.GetStack(from).GetTopCards();
    res.GetStack(to).AddCards(std::move(slice));
    res.Normalize();
    return res;
}


std::set<TGameState> TGameSolver::PossibleStates(const TGameState &state) {
    std::set<TGameState> possible;
    for (size_t from = 0; from < TGameState::StackCount; ++from) {
        if (state.GetStack(from).IsEmpty()) {
            continue;
        }
        for (size_t to = 0; to < TGameState::StackCount; ++to) {
            if (from == to) {
                continue;
            }
            if (!IsAbleToAction(state, from, to)) {
                continue;
            }
            auto res = Action(state, from, to);
            possible.insert(res);
        }
    }
    return possible;
}


ssize_t TGameSolver::Solve(const TGameState &startState) {
    std::map<TGameState, size_t> DistanceToState_;
    std::set<std::pair<size_t, TGameState>> PredictDistance_;

    DistanceToState_[startState] = 0;
    PredictDistance_.insert(std::make_pair(0 + startState.NumOfClosed(), startState));

    while (!PredictDistance_.empty()) {
        auto state = PredictDistance_.begin()->second;
        PredictDistance_.erase(PredictDistance_.begin());
        size_t distance = DistanceToState_[state];

        for (const auto &newState : PossibleStates(state)) {
            if (newState.IsFinal()) {
                return distance + 1;
            }
            if (!DistanceToState_.contains(newState)) {
                PredictDistance_.erase(std::make_pair(DistanceToState_[newState] + newState.NumOfClosed(), newState));
                DistanceToState_[newState] = distance + 1;
                PredictDistance_.insert(std::make_pair(distance + 1 + newState.NumOfClosed(), newState));
            }
        }
    }
    return -1;
}
