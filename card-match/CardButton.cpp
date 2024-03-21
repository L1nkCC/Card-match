//
// Created by conno on 10/2/2022.
//

#include "CardButton.h"

CardButton::CardButton(std::shared_ptr<Card> n_card, std::shared_ptr<QPushButton> n_button) {
    card = n_card;
    button = n_button;

}

