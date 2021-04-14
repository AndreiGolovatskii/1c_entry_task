#include "game_state.hh"

std::vector<TCard> TGameState::TStack::GetTopCards() {
    std::vector<TCard> res;
    for (size_t i = Content_.size(); i > Closed_; --i) {
        if (res.empty() || res.back().IsPrevTo(Content_.back())) {
            res.push_back(Content_.back());
            Content_.pop_back();
        } else {
            break;
        }
    }
    if (Closed_ > 0 && Closed_ == Content_.size()) {
        Closed_--;
    }
    return res;
}


void TGameState::TStack::AddCards(std::vector<TCard> &&cards) {
    assert(!cards.empty());
    assert(Content_.empty() || Content_.back().IsNextTo(cards.back()));

    while (!cards.empty()) {
        Content_.push_back(cards.back());
        cards.pop_back();
    }

    if (Content_.size() >= 9 && Content_.back() == TCard(0) && GetTopCardView() == TCard(8)) {
        GetTopCards();// remove all 9 cards
    }
}


TCard TGameState::TStack::GetTopCardView() const {
    if (Content_.empty()) {
        throw std::logic_error{"No cards"};
    }
    TCard last = Content_.back();
    for (size_t i = Content_.size() - 1; i > Closed_; --i) {
        if (last.IsPrevTo(Content_[i - 1])) {
            last = Content_[i - 1];
        } else {
            break;
        }
    }

    return last;
}

size_t TGameState::NumOfClosed() const {
    size_t cnt = 0;
    for (size_t i = 0; i < StackCount; ++i) {
        cnt += Stacks_[i].Closed();
    }
    return cnt;
}

bool TGameState::operator==(const TGameState &other) const {
    for (size_t i = 0; i < StackCount; ++i) {
        if (!(Stacks_[i] == other.Stacks_[i])) {
            return false;
        }
    }
    return true;
}
