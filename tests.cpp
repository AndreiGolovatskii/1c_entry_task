#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "game_solver.hh"
#include "game_state.hh"


std::vector<TCard> ToCards(std::vector<int> &&content) {
    std::vector<TCard> res;
    for (int i : content) {
        res.emplace_back(i);
    }
    return res;
}


TEST(State, testStack) {
    ASSERT_TRUE(TGameState::FinalState().IsFinal());


    TGameState::TStack stack{ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0})};
    ASSERT_EQ(stack.GetTopCardView(), TCard{0});
    for (size_t i = 0; i < 8; ++i) {
        stack.AddCards(stack.GetTopCards());
    }
    // Now all cards open
    // And all cards removed
    ASSERT_TRUE(stack.IsEmpty());
}

TEST(Solver, Simple) {
    TGameSolver solver;

    TGameState state_simple(std::vector<std::vector<TCard>>{ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
                                                            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
                                                            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
                                                            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
                                                            {},
                                                            {},
                                                            {},
                                                            {}});

    ASSERT_TRUE(solver.Solve(state_simple) != -1);// solution exists


    TGameState state_hopeless(std::vector<std::vector<TCard>>{
            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),

            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
            ToCards({0, 1, 2, 3, 4, 5, 6, 7, 8}),
    });

    ASSERT_TRUE(solver.Solve(state_hopeless) == -1);// solution NOT exists
}

TEST(Solver, SevenStackOneEmpty) {
    TGameState state_hard(std::vector<std::vector<TCard>>{ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0}),
                                                          ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0}),
                                                          ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0}),
                                                          ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0}),

                                                          ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0}),
                                                          ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0}),
                                                          ToCards({8, 7, 6, 5, 4, 3, 2, 1, 0}),
                                                          {}});

    ASSERT_TRUE(TGameSolver::Solve(state_hard) != -1);// solution NOT exists
}