#ifndef CARD_H
#define CARD_H

#include <QIcon>
#include <QString>

class Card {
public:
    const static int NUMBER_OF_SUITS = 4;
    const static int NUMBER_OF_RANKS = 13;
    enum Suit{
        diamond = 1, hearts, spades, clubs
    };
    enum Rank{
        ace = 1, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king
    };

    Suit getSuit(){
        return cSuit;
    }
    Rank getRank(){
        return cRank;
    }

    bool operator<(Card b){
        if(cSuit*13+cRank < b.getSuit()*13+b.getRank())
            return true;
        else
            return false;
    }
    bool operator==(Card b){
        if(cRank == b.getRank() && cSuit==b.getSuit())
            return true;
        return false;
    }

    explicit Card(Suit suit, Rank rank, QString iconName, QObject* parent = nullptr);

    [[nodiscard]] const QIcon& icon() const {
        static QIcon back(":/back.png");
        return m_up ? m_front : back;
    }

    void flip() {
        m_up = !m_up;
    };
    void down(){
        m_up = false;
    };


private:
    Suit cSuit;
    Rank cRank;
    QIcon m_front;
    bool m_up = true;
};

#endif // CARD_H
