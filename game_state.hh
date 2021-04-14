#pragma once

#include <array>
#include <map>
#include <set>
#include <vector>
#include <cassert>
#include <algorithm>


class TCard {
    int num_value_;

public:
    explicit TCard(int num) : num_value_(num) {
    }

    [[nodiscard]] bool IsPrevTo(const TCard &card) const {
        return num_value_ + 1 == card.num_value_;
    }
    [[nodiscard]] bool IsNextTo(const TCard &card) const {
        return card.num_value_ + 1 == num_value_;
    }
    bool operator==(const TCard &other) const {
        return num_value_ == other.num_value_;
    }
    bool operator<(const TCard &other) const {
        return num_value_ < other.num_value_;
    }
};


class TGameState {
public:
    class TStack {
        std::vector<TCard> Content_;
        size_t Closed_;

    public:
        TStack() : Closed_(0) {
        }

        template<class T>
        explicit TStack(const T &content) {
            Content_ = content;
            Closed_ = content.empty() ? 0 : content.size() - 1;
        }

        [[nodiscard]] size_t Closed() const {
            return Closed_;
        }

        std::vector<TCard> GetTopCards();

        void AddCards(std::vector<TCard> &&cards);

        [[nodiscard]] bool IsEmpty() const {
            return Content_.empty();
        }

        [[nodiscard]] bool AbleToAdd(const TCard &card) const {
            return Content_.empty() || Content_.back().IsNextTo(card);
        }

        [[nodiscard]] TCard GetTopCardView() const;

        bool operator<(const TStack &other) const {
            if (Content_ == other.Content_) {
                return Closed_ < other.Closed_;
            }
            return Content_ < other.Content_;
        }

        bool operator==(const TStack &other) const {
            return Content_ == other.Content_ && Closed_ == other.Closed_;
        }

        bool operator<=(const TStack &other) const {
            return operator==(other) || operator<(other);
        }

        bool operator>(const TStack &other) const {
            return !operator<=(other);
        }
    };

    template<class T>
    explicit TGameState(const T &content);

    static TGameState FinalState() {
        return TGameState{std::vector<std::vector<TCard>>{StackCount}};
    }

    TStack &GetStack(size_t id) {
        return Stacks_[id];
    }

    const TStack &GetStack(size_t id) const {
        return Stacks_[id];
    }

    void Normalize() {
        if (!std::is_sorted(Stacks_.begin(), Stacks_.end())) {
            std::sort(Stacks_.begin(), Stacks_.end());
        }
    }

    [[nodiscard]] bool IsFinal() const {
        return std::all_of(Stacks_.begin(), Stacks_.end(), [](const auto &stack) { return stack.IsEmpty(); });
    }

    [[nodiscard]] size_t NumOfClosed() const;

    bool operator==(const TGameState &other) const;

    bool operator<(const TGameState &other) const {
        return Stacks_ < other.Stacks_;
    }

public:
    static const size_t StackCount = 8;

private:
    std::array<TStack, StackCount> Stacks_;
};


template<class T>
TGameState::TGameState(const T &content) {
    assert(content.size() == StackCount);
    for (size_t i = 0; i < StackCount; ++i) {
        Stacks_[i] = TStack{content[i]};
    }
    Normalize();
}
