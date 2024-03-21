#include <cassert>
#include <algorithm>
#include <random>
#include <chrono>
#include "CardButton.h"
#include <iostream>
#include <QTimer>
#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    const QString card_base_name_list[] = {":/diamond_", ":/hearts_", ":/spades_", ":/clubs_"};

    const int NUMBER_OF_COLUMNS = 13;
    const QSize CARD_SIZE(71, 94);
    for (int s = 0; s < Card::NUMBER_OF_SUITS; ++s) {
        for(int r = 0; r < Card::NUMBER_OF_RANKS; r++){
            const QString card_name = card_base_name_list[s] + QString::number(r+1);
            auto card = std::make_shared<Card>(static_cast<Card::Suit>(s+1), static_cast<Card::Rank>(r+1), card_name);
            auto snubButton = std::make_shared<QPushButton>(this);
            auto button = std::make_shared<CardButton>(card, snubButton);

            card->flip();
            m_unmatchedCards.insert(card);

            m_cards.push_back(card);
            button->button->setIcon(card->icon());
            button->button->setIconSize(CARD_SIZE);
            button->button->setFlat(true);
            m_buttons.push_back( std::move(button));
            int row = (s*Card::NUMBER_OF_RANKS+r) / NUMBER_OF_COLUMNS;
            int column = (s*Card::NUMBER_OF_RANKS+r) % NUMBER_OF_COLUMNS;
            auto new_cButton = m_buttons.back().get();
            QPushButton* new_button = new_cButton->button.get();
            ui->gridLayout->addWidget(new_button, row, column);

            QObject::connect(new_button, SIGNAL(clicked()),
                            new_cButton, SLOT(select()));
            QObject::connect(new_cButton, SIGNAL(selecting(std::shared_ptr<Card>)),
                             this, SLOT(flipped(std::shared_ptr<Card>)));
        }


    }
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(Card::NUMBER_OF_RANKS*Card::NUMBER_OF_SUITS/2);
    updatePBar();

    QObject::connect(ui->shuffleButton, SIGNAL(clicked()),
                     this, SLOT(shuffle()));
    QObject::connect(ui->quitButton, SIGNAL(clicked()),
                     this, SLOT(quit()));
    shuffle();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::quit() {
    QApplication::quit();
}

void MainWindow::updatePBar(){
    ui->label->setText( QString("Guesses: ")+ QString::number(m_guesses) + "     Matches: " + QString::number(m_matchedCards.size()/2));
    ui->progressBar->setValue(m_matchedCards.size()/2);
}

void MainWindow::sync() {
    // synchronize the button icons with the cards
    auto cardIt = m_cards.begin();
    auto buttonIt = m_buttons.begin();
    while (cardIt != m_cards.end()) {
        (*buttonIt)->button->setIcon(cardIt->get()->icon());
        ++cardIt;
        ++buttonIt;
    }
    assert(buttonIt == m_buttons.end());
}

void MainWindow::sort() {
    std::sort(m_cards.begin(), m_cards.end());
    sync();
}

void MainWindow::shuffle() {

    //disconnect
    for(int s = 0; s < Card::NUMBER_OF_SUITS; s++){
        for(int r = 0; r < Card::NUMBER_OF_RANKS; r++){
            QObject::disconnect(ui->gridLayout->itemAtPosition(s, r)->widget(), nullptr, nullptr, nullptr);
            QObject::disconnect(m_buttons[s*13+r].get(), nullptr, nullptr, nullptr);
        }
    }



    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_cards.begin(), m_cards.end(), std::default_random_engine(seed));

    m_matchedCards.clear();
    m_unmatchedCards.clear();
    for(int s = 0; s < Card::NUMBER_OF_SUITS; s++){
        for(int r = 0; r < Card::NUMBER_OF_RANKS; r++){
            m_buttons[s*13+r]->card = m_cards[s*13+r];
            m_cards[s*13+r]->down();
            m_unmatchedCards.insert(m_cards[s*13+r]);
            auto new_button = m_buttons[s*13+r]->button.get();
            auto new_cButton = m_buttons[s*13+r].get();
            QObject::connect(new_button, SIGNAL(clicked()),
                     new_cButton, SLOT(select()));
            QObject::connect(new_cButton, SIGNAL(selecting(std::shared_ptr<Card>)),
                     this, SLOT(flipped(std::shared_ptr<Card>)));
        }
    }
    updatePBar();
    sync();
}

void MainWindow::flipped(std::shared_ptr<Card> card) {
    if(m_card1 == nullptr){
        m_card1 = card;
        for(int c = 0; c < m_buttons.size(); c++){
            if(m_buttons[c]->card == m_card1){
                m_buttons[c]->button->blockSignals(true);
            }
        }
    }else{
        m_card2 = card;
        m_guesses ++;

        if(m_card1 == m_card2) {
            std::cout<<"Error: m_card1 == m_card2"<<std::endl;
            return;
        }
        if(m_card1->getRank() == m_card2->getRank()) {
            //add to matchset
            m_matchedCards.insert(m_card1);
            m_matchedCards.insert((m_card2));
            m_unmatchedCards.erase(m_unmatchedCards.find(m_card1));
            m_unmatchedCards.erase(m_unmatchedCards.find(m_card2));

            //block signals
            for (int c = 0; c < m_buttons.size(); c++) {
                if (m_buttons[c]->card == m_card1)
                    m_buttons[c]->button->blockSignals(true);
                if (m_buttons[c]->card == m_card2)
                    m_buttons[c]->button->blockSignals(true);
            }

            //win Condition
            if (m_unmatchedCards.empty())
                win();//////////////////////////////////////
        }else{
            for (int c = 0; c < m_buttons.size(); c++) {
                    m_buttons[c]->button->blockSignals(true);
            }
            QTimer::singleShot(3500,this, SLOT(wait()));
        }
        m_card1 = nullptr;
        m_card2 = nullptr;
    }
    updatePBar();
    sync();
}

void MainWindow::wait(){
    for(int c = 0; c < m_buttons.size(); c++){
        if(m_unmatchedCards.find(m_buttons[c]->card) != m_unmatchedCards.end()){
            m_buttons[c]->button->blockSignals(false);
            m_buttons[c]->card->down();
        }
    }
    sync();
}

void MainWindow::win(){
    ui->label->setText(QString("You Win!"));
    updatePBar();
    QTimer::singleShot(3000,this, SLOT(shuffle()));
}