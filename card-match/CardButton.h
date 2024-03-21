//
// Created by conno on 10/2/2022.
//

#ifndef CARD_MATCH_CARDBUTTON_H
#define CARD_MATCH_CARDBUTTON_H

#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QPushButton>
#include <vector>
#include <memory>
#include "card.h"

class CardButton : public QObject{
    Q_OBJECT
    public:

        CardButton(std::shared_ptr<Card> n_card, std::shared_ptr<QPushButton>n_button);

        std::shared_ptr<QPushButton> button;
        std::shared_ptr<Card> card;

    public slots:
        void select(){
            card->flip();
            selecting(card);
        };
    signals:
        void selecting(std::shared_ptr<Card> card);
};


#endif //CARD_MATCH_CARDBUTTON_H
