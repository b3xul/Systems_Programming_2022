//
//  game.cpp
//  
//
//  Created by Mac on 14/01/20.
//
//

/* Gioco online tra due giocatori, gestione di più thread
 
 Implementare classe Challenge che aveva, oltre al costruttore, i seguenti metodi:
 
 -void accept( Boolean response)
 Indica il responso dello sfidato alla richiesta di giocare
 
 -void result1( int score)
 Indica lo score dello sfidante
 
 -void result2( int score)
 Indica lo score dello sfidato
 
 -int winner()
 Ritorna:
 -0 se è pari
 -1 se ha vinto lo sfidante
 -2 se ha vinto lo sfidato
 - -1 se la richiesta è stata rifiutata
 
 Accept deve precedere result1 e result2,  che invece non hanno ordine di chiamata tra loro, e tutti e 3 possono essere chiamati una sola volta, la violazione dei vincoli genera eccezione.
 
 Winner si mette in attesa senza consumare CPU se il gioco è in corso o non è ancora cominciato */

#include <stdio.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>

using namespace std;

class Challenge

{
    // list of variables
    int score1;
    int score2;
    bool okresult1;
    bool okresult2;
    bool okgame;
    bool request;
    //bool gameended;
    bool refused;
    mutex m;
    condition_variable start;
    thread giocatore1;
    
    
public:
    Challenge(): okgame(false), okresult2(false), okresult1(false), refused(false), request(false) // variables to initialize
    {
        giocatore1 = thread([this](){request = true; result1(2);});
    }
    
    ~Challenge()
    {
        if(giocatore1.joinable())
        {
            giocatore1.join();
        }
    }
    
    void accept(bool response)
    {
        lock_guard<mutex> lg(m);
        if(okgame)
        {
            cout << "Game already accepted" << endl;
        }
        if(request && response)
        {
            okgame = true;
        }
        else
        {
            refused = true;
        }
        start.notify_all();
    }
    
    void result1(int score)
    {
        
        unique_lock<mutex> ul(m);
        start.wait(ul, [this](){return (okgame);});
        cout << "Setting result 1..." << endl;
        if(okresult1)
        {
            cout << "Score1 is already set!" << endl ;
        }
        else
        {
            score1 = score;
            okresult1 = true;
            cout << "Result 1 set." << endl;
            start.notify_one();
        }
        
    }
    
    void result2(int score)
    {
        unique_lock<mutex> ul(m);
        start.wait(ul, [this](){return (okgame);});
        cout << "Setting result 2..." << endl;
        if (okresult2)
        {
            cout << "Score2 is already set!" << endl ;

        }
        else
        {
            score2 = score;
            okresult2 = true;
            cout << "Result 2 set." << endl;
            start.notify_one();
        }
    }
    
    int winner()
    {
        unique_lock<mutex> ul(m);
        //cout << "okgame is " << okgame << endl;
        //cout << "okresult1 and okresult2:" << okresult1 << "|" << okresult2 << endl;
        start.wait(ul, [this]() { return (refused || (okgame && okresult1 && okresult2)); });
        
        /*cout << "okgame is " << okgame << endl;
        cout << "okresult1 and okresult2: " << okresult1 << " | " << okresult2 << endl;
        cout << "score1 and score2:" << score1 <<  " | " << score2 << endl;*/
        
        if(refused)
        {
            return -1;
        }
        
        if (score1 > score2)
        {
            return 1;
        }
        else if (score1 < score2)
        {
            return 2;
        }
        else if (score1 == score2)
        {
            return 0;
        }
        else
        {
            return 1000;
        }
    }
    
    
};


int main()
{
    Challenge c;
    thread giocatore2;
    giocatore2 = thread([&c](){c.accept(true); c.result2(5);});
    cout << "Winner is gamer:" << c.winner() << endl;
    giocatore2.join();
}
