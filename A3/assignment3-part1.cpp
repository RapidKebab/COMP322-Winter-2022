using namespace std;
#include <string> 
#include <iostream>

#include <cstdlib>
#include <ctime>

#include <vector>

enum Rank
{
    ACE = '1',
    TWO = '2',
    THREE = '3',
    FOUR = '4',
    FIVE = '5',
    SIX = '6',
    SEVEN = '7',
    EIGHT = '8',
    NINE = '9',
    TEN = 'T',
    JACK = 'J',
    QUEEN = 'Q',
    KING = 'K',
};

enum Type
{
    DIAMONDS = 'D',
    SPADES = 'S',
    CLUBS = 'C',
    HEARTS = 'H',
};

class Card
{
    // Fields
    private:
        Rank rank;
        Type type;

    // Methods
    public:
        Card(Rank r, Type t);
        void setRank(Rank r);
        Rank getRank();
        void setType(Type t);
        Type getType();
        int getValue();
        void displayCard();
};

class Hand
{

    // Field
    private: 
        vector<Card> hand;
        int countAces = 0;

    // Methods
    public:
        vector<Card> getVecHand();
        void add(Card c);
        void clear();
        int getTotal() const;
};

class Deck
{

    // Field
    private:
        vector<Card> deck;

    // Methods
    public:
        vector<Card> getDeck();
        void populate();
        void shuffle();
        Card deal();
        void clear();
};

class AbstractPlayer
{

    // Field
    public:
        Hand hand;

    // Methods
    public:
        Hand getHand();
        virtual bool isDrawing() const = 0;
        bool isBusted();
        void displayHand();
};

enum Status {
    LOST,
    WIN,
    TIE,
};

class HumanPlayer : public AbstractPlayer
{

    // Inherited methods and fields : hand, isDrawing(), isBusted()

    // Methods
    public:
        bool isDrawing() const override;
        void announce(Status status);
};

class ComputerPlayer : public AbstractPlayer
{

    // Inherited methods and fields : hand, isDrawing(), isBusted()

    public:
        bool isDrawing() const override;
};

class BlackJackGame
{

    // Fields
    private:
        Deck m_deck;
        ComputerPlayer m_casino;

    public:
        Status play();
        void endRound();
};


// Card

Card::Card(Rank r, Type t)
{
    this->rank = r;
    this->type = t;
};

Rank Card::getRank()
{
    return this->rank;
}

Type Card::getType()
{
    return this->type;
}


void Card::setRank(Rank r)
{
    this->rank = r;
};

void Card::setType(Type t)
{
    this->type = t;
};

int Card::getValue()
{
    int value;

    // Convert card.rank char to int according to its value

    if (this->rank == 'T' || this->rank == 'J' ||this->rank == 'Q' || this->rank == 'K')
    {
        value = 10;
    }
    else
    {
        // convert char to int : '1' -> 1
        value = this->rank - '0';
    }
    
    return value;
};

void Card::displayCard()
{
    string rankString;

    if (this->rank == 'T')
    {
        rankString = "10";
    }
    else
    {
        std::string s(1, this->rank);
        rankString = s;
    }

    std::string typeString(1, this->type);
    string cardString = rankString + typeString;

    cout << cardString;
};

/////////////////////////////////////////////////////

// Hand

void Hand::add(Card c)
{

    if (c.getRank() == '1')
    {
        this->countAces++;
    }

    hand.push_back(c);
};

void Hand::clear()
{
    (this->countAces) = 0;
    (this->hand).clear();
};

int Hand::getTotal() const
{
    vector<Card> h = this->hand;

    int total = 0;
    for (Card card: h)
    {
        total += card.getValue();
    }

    int countA = this->countAces;

    while (total != 21 && countA > 0)
    {
        // Make aces count for 11 instead of 1
        if (total <= 11)
        {
            total += 10;
            countA -= 1;
        }
        else
        {
            break;
        }
    }

    return total;
};

vector<Card> Hand::getVecHand()
{
    return this->hand;
}

////////////////////////////////////////////////////

// Deck

vector<Card> Deck::getDeck()
{
    return this->deck;
};

void Deck::populate()
{
    Rank vRank[] = {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
    Type vType[] = {CLUBS, DIAMONDS, SPADES, HEARTS};

    // Create cards 1 to 9 of every type
    for (Rank r : vRank)
    {
        for (Type t : vType)
        {
            Card newCard(r, t);
            (this->deck).push_back(newCard);
        }
    }
};

void Deck::shuffle()
{
    for (int i=0; i<deck.size(); i++)
    {
        int indexToSwap = rand() % deck.size();   // random index between 0 and 51
        Card temp = deck[indexToSwap];
        deck[indexToSwap] = deck[i];
        deck[i] = temp;
    }
}

Card Deck::deal()
{
    Card cDealt = deck.back();
    (this->deck).pop_back();
    return cDealt;
}

void Deck::clear()
{
    this->deck.clear();
}

////////////////////////////////////////////////////

// Abstract Player

Hand AbstractPlayer::getHand()
{
    return this->hand;
};

bool AbstractPlayer::isBusted()
{
    return (this->hand.getTotal() > 21);
};

void AbstractPlayer::displayHand()
{
    for (Card card: (this->hand).getVecHand())
    {
        card.displayCard();
        cout << " ";
    }

    cout << "[" << (this->hand).getTotal() << "]";
};

////////////////////////////////////////////////////

// Human Player

bool HumanPlayer::isDrawing() const
{
    cout << "Do you want to draw ? (y/n) " << endl;

    string inputPlayer;
    bool isValid = true;
    

    while (true){
        getline(cin, inputPlayer);
        if (inputPlayer == "y")
        {
            return true;
            isValid=true;
        }   
        else if (inputPlayer == "n")
        {
            isValid=true;
            return false;
        }
        else
        {
            cout << "Input invalid. Press 'y' to draw a card, 'n' to stop" << endl;
            isValid = false;
        }
    }

    //return hand.getTotal() <= 15;
};

void HumanPlayer::announce(Status status)
{
    if (status == WIN)
    {
        cout << "Player wins." << endl;
    }
    else if (status == LOST)
    {
        cout << "Casino Wins." << endl;
    }
    else if (status == TIE)
    {
        cout << "Push: No one wins." << endl;
    }
};



////////////////////////////////////////////////////

// ComputerPlayer

bool ComputerPlayer::isDrawing() const
{
    return hand.getTotal() <= 16;
};
 
////////////////////////////////////////////////////

// BlackJackGame

void BlackJackGame::endRound()
{
    m_casino.hand.clear();
    m_deck.clear();
}

Status BlackJackGame::play()
{
    // Create deck, populate, shuffle
    m_deck.populate();
    m_deck.shuffle();
    m_deck.shuffle();
    m_deck.shuffle();
    
    // create player
    HumanPlayer *player = new HumanPlayer();

    // Player is dealt 2 open card.
    (*player).hand.add( m_deck.deal() );
    (*player).hand.add( m_deck.deal() );

    cout << "Player: ";
    (*player).displayHand();
    cout << endl;

    // Computer is dealt one card.
    m_casino.hand.add( m_deck.deal() );

    cout << "Casino: ";
    m_casino.displayHand();
    cout << endl;

    // while Player chooses to draw a card
    while ((*player).isDrawing())
    {
        (*player).hand.add( m_deck.deal() );

        cout << "Player: ";
        (*player).displayHand();
        cout << endl;

    }

            // Check if busted.
        if ((*player).isBusted())
        {
            cout << "Player busts." << endl;
            (*player).announce(LOST);
            endRound();
            return LOST;
        }

    // Computer draws card until he reaches a sum of 16
    while (m_casino.isDrawing())
    {
        m_casino.hand.add( m_deck.deal() );

        cout << "Casino: ";
        m_casino.displayHand();
        cout << endl;
    }

    // Check if the computer busted
    if (m_casino.isBusted())
    {
        (*player).announce(WIN);
        endRound();
        return WIN;
    }
    else 
    {
        // compare values to see who's the closest to 21
        int playerDiff = (21 - (*player).hand.getTotal());
        int compDiff = (21 - m_casino.hand.getTotal());

        if (playerDiff > compDiff)
        {
            (*player).announce(LOST);
            endRound();
            return LOST;
        }
        else if (compDiff > playerDiff)
        {
            (*player).announce(WIN);
            endRound();
           return WIN;
        }
        else if (compDiff == playerDiff)
        {
            (*player).announce(TIE);
            endRound();
            return TIE;
        }
    }

    // end of round. Clear casino and player's hand. Shuffle the deck.
    endRound();

    return TIE;
}
int main()
{
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
    game.play();
    // Check whether the player would like to play another round
    cout << "Would you like another round? (y/n): ";
    cin >> answer;
    cout << endl << endl;
    playAgain = (answer == 'y' ? true : false);
    }
    cout <<"Gave over!";
    return 0;
}

