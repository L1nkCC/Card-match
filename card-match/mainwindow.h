#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <vector>
#include <memory>
#include <set>
#include "card.h"
#include "CardButton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

    void win();

    void updatePBar();

private slots:

    static void quit();

    void sync();

    void sort();

    void shuffle();

    void wait();

public slots:

    void flipped(std::shared_ptr<Card> card);

private:
    Ui::MainWindow* ui;
    std::vector<std::shared_ptr<CardButton>> m_buttons;
    std::vector<std::shared_ptr<Card>> m_cards;

    std::set<std::shared_ptr<Card>> m_matchedCards;
    std::set<std::shared_ptr<Card>> m_unmatchedCards;
    std::shared_ptr<Card> m_card1;
    std::shared_ptr<Card> m_card2;
    unsigned int m_guesses = 0;
};

#endif // MAINWINDOW_H