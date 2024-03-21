#include "card.h"
#include <cassert>

Card::Card(Suit suit, Rank rank, QString iconName, QObject* parent) : m_front(iconName) {
    cSuit = suit;
    cRank = rank;
    assert(!m_front.isNull());

}