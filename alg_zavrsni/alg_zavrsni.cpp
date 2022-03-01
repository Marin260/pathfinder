#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
using namespace std;

const int inf = INT_MAX;
int startVertices = 0;

const int N = 1600; // velicina matrice

struct Vrh {
    bool check;
    sf::RectangleShape square;
    vector<int> susjedi;

    Vrh() {
        check = false;
        square.setSize(sf::Vector2f(25.f, 25.f));
        square.setPosition(sf::Vector2f(0.f, 1.f));
        square.setFillColor(sf::Color::White);
    }
    Vrh(sf::Vector2f btnSize, sf::Vector2f btnPosition) {
        check = false;
        square.setSize(btnSize);
        square.setPosition(btnPosition);
        square.setFillColor(sf::Color::White);
    }

    bool hover(sf::RenderWindow& window) {
        float mouseX = sf::Mouse::getPosition(window).x; // x coordinate of the mouse
        float mouseY = sf::Mouse::getPosition(window).y; // y coordinate of the mouse

        float btnPosX = square.getPosition().x; // x coordinate of the button
        float btnPosY = square.getPosition().y; // y coordinate of the button

        float btnPositionWidth = btnPosX + square.getLocalBounds().width; // button width boundary
        float btnPositionHeight = btnPosY + square.getLocalBounds().height; // button height boundary

                                                                                           // if the mouse is within the button || smaller than lower end, greater than upper end (x or y)
        if (mouseX < btnPositionWidth && mouseX > btnPosX&& mouseY < btnPositionHeight && mouseY > btnPosY) {
            return true;
        }
        else return false;
    }
};
void reset(Vrh* vrhovi, int checked[], int** mat);

int main()
{
    int** next = new int* [N];
    int** mat = new int* [N];
    for (int i = 0; i < N; i++) {
        mat[i] = new int[N];
        next[i] = new int[N];
    }

    // stvaranje matrice
    Vrh* sviVrhovi = new Vrh[N];

    // Postavaljanje pozicije svakog vrha na ekranu
    sf::Vector2f pos(0.f, 0.f);
    for (int i = 0; i < N; i++) {
        if (i % 40 == 0 && i !=0) {
            pos.x = 0.f;
            pos.y += 26.f;
        }
        sviVrhovi[i].square.setPosition(1.f + pos.x, 1.f + pos.y);
        pos.x += 26.f;
        // unos susjeda svakog cvora
        if (i % 40 != 0 && (i > 40 && i < 1560) && i % 40 != 39) {
            sviVrhovi[i].susjedi.push_back(i + 1);
            sviVrhovi[i].susjedi.push_back(i - 1);
            sviVrhovi[i].susjedi.push_back(i + 40);
            sviVrhovi[i].susjedi.push_back(i - 40);
        }
        if (i % 40 == 0) {
            if (i > 40)
                sviVrhovi[i].susjedi.push_back(i - 40);
            if (i < 1560)
                sviVrhovi[i].susjedi.push_back(i + 40);
            sviVrhovi[i].susjedi.push_back(i + 1);
        }
        if (i % 40 == 39) {
            if (i > 40)
                sviVrhovi[i].susjedi.push_back(i - 40);
            if (i < 1560)
                sviVrhovi[i].susjedi.push_back(i + 40);
            sviVrhovi[i].susjedi.push_back(i - 1);
        }
        if (i < 40 && i % 40 != 0 && i % 40 != 39) {
            sviVrhovi[i].susjedi.push_back(i - 1);
            sviVrhovi[i].susjedi.push_back(i + 1);
            sviVrhovi[i].susjedi.push_back(i + 40);
        }
        if (i > 1560 && i % 40 != 0 && i % 40 != 39) {
            sviVrhovi[i].susjedi.push_back(i - 1);
            sviVrhovi[i].susjedi.push_back(i + 1);
            sviVrhovi[i].susjedi.push_back(i - 40);
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = (i == j) ? 0 : inf;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < sviVrhovi[i].susjedi.size(); j++) {
            mat[i][sviVrhovi[i].susjedi[j]] = 1;
        }
    }
    
    // stvaranje novog eventa
    sf::Event event;
    int checked[2] = {0, 0};

    sf::RenderWindow window(sf::VideoMode(1041, 1041), "Najkraci put");
    while (window.isOpen())
    {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                for (int i = 0; i < N; i++) {
                    if (sviVrhovi[i].hover(window) && startVertices < 2 && sviVrhovi[i].check == false) {
                        // test susjeda
                        //for (int k = 0; k < sviVrhovi[i].susjedi.size(); k++)
                        //sviVrhovi[sviVrhovi[i].susjedi[k]].square.setFillColor(sf::Color::Blue);


                        // Postavljanje pocetnog i zavrsnog cvora
                        sviVrhovi[i].square.setFillColor(sf::Color::Red);
                        checked[startVertices] = i;
                        startVertices++;
                        sviVrhovi[i].check = true;
                    }
                    else if (sviVrhovi[i].hover(window) && sviVrhovi[i].check == true && (i == checked[1] || i == checked[0])) {
                        // vracanje pocetnog i zavrsnog cvora na pocetno stanje
                        sviVrhovi[i].square.setFillColor(sf::Color::White);
                        startVertices--;
                        checked[startVertices] = 0;
                        sviVrhovi[i].check = false;
                    }
                    else if (sviVrhovi[i].hover(window) && sviVrhovi[i].check == false && startVertices == 2 && i != checked[1] && i != checked[0]) {
                        // Nakon dva klika postavljanje prepreka crnom bojom
                        sviVrhovi[i].square.setFillColor(sf::Color::Black);
                        sviVrhovi[i].check = true;
                        for (int j = 0; j < sviVrhovi[i].susjedi.size(); j++) {
                            mat[i][sviVrhovi[i].susjedi[j]] = inf;
                            mat[sviVrhovi[i].susjedi[j]][i] = inf;
                        }
                    }
                    else if (sviVrhovi[i].hover(window) && sviVrhovi[i].check == true && startVertices == 2 && i != checked[1] && i != checked[0]) {
                        // vracanje prepreka u pocetno stanje
                        sviVrhovi[i].square.setFillColor(sf::Color::White);
                        sviVrhovi[i].check = false;
                        for (int j = 0; j < sviVrhovi[i].susjedi.size(); j++) {
                            mat[i][sviVrhovi[i].susjedi[j]] = 1;
                            mat[sviVrhovi[i].susjedi[j]][i] = 1;
                        }
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                    for (int i = 0; i < N; i++) {
                        for (int j = 0; j < N; j++) {
                            if (mat[i][j] == inf)
                                next[i][j] = -1;
                            else
                                next[i][j] = j;
                        }
                    }

                    cout << checked[0] << "--->" << checked[1] << endl;

                    for (int k = 0; k < N; k++) {
                        for (int i = 0; i < N; i++) {
                            for (int j = 0; j < N; j++) {
                                if (mat[i][k] == inf || mat[k][j] == inf)
                                    continue;
                                if (mat[i][j] > mat[i][k] + mat[k][j]) {
                                    mat[i][j] = mat[i][k] + mat[k][j];
                                    next[i][j] = next[i][k];
                                }
                            }
                        }
                    }

                    cout << "Udaljenost od grada " << checked[0] << " do grada " << checked[1] << " iznosi " << mat[checked[0]][checked[1]] << endl;
                    vector<int> path = { checked[0] }; // najkraci put od cvora do cvora

                    while (checked[0] != checked[1]) {
                        checked[0] = next[checked[0]][checked[1]];
                        path.push_back(checked[0]);
                    }
                    cout << endl;
                    for (int i = 0; i < path.size(); i++) {
                        // Oznacavanje rnakrace rute
                        cout << path[i];
                        if (i != path.size() - 1) cout << "->";
                        if (sviVrhovi[path[i]].check == false) {
                            sviVrhovi[path[i]].square.setFillColor(sf::Color::Blue);
                        }
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    reset(sviVrhovi, checked, mat);
                }

            }
        }

        window.clear();
        for (int j = 0; j < N; j++) 
            window.draw(sviVrhovi[j].square);
        
        window.display();
    }

    for (int i = 0; i < N; i++) {
        delete[] mat[i];
        delete[] next[i];
    }
    delete[] mat;
    delete[] next;

    return 0;
}

void reset(Vrh* sviVrhovi, int checked[], int** mat) {
    // vracanje matrice na pocetne postavke
    startVertices = 0;
    checked[0] = 0;
    checked[1] = 0;
    for (int i = 0; i < N; i++) {
        sviVrhovi[i].check = false;
        sviVrhovi[i].square.setFillColor(sf::Color::White);
        for (int j = 0; j < sviVrhovi[i].susjedi.size(); j++)
            sviVrhovi[i].susjedi.pop_back();
        // unos susjeda svakog cvora
        if (i % 40 != 0 && (i > 40 && i < 1560) && i % 40 != 39) {
            sviVrhovi[i].susjedi.push_back(i + 1);
            sviVrhovi[i].susjedi.push_back(i - 1);
            sviVrhovi[i].susjedi.push_back(i + 40);
            sviVrhovi[i].susjedi.push_back(i - 40);
        }
        if (i % 40 == 0) {
            if (i > 40)
                sviVrhovi[i].susjedi.push_back(i - 40);
            if (i < 1560)
                sviVrhovi[i].susjedi.push_back(i + 40);
            sviVrhovi[i].susjedi.push_back(i + 1);
        }
        if (i % 40 == 39) {
            if (i > 40)
                sviVrhovi[i].susjedi.push_back(i - 40);
            if (i < 1560)
                sviVrhovi[i].susjedi.push_back(i + 40);
            sviVrhovi[i].susjedi.push_back(i - 1);
        }
        if (i < 40 && i % 40 != 0 && i % 40 != 39) {
            sviVrhovi[i].susjedi.push_back(i - 1);
            sviVrhovi[i].susjedi.push_back(i + 1);
            sviVrhovi[i].susjedi.push_back(i + 40);
        }
        if (i > 1560 && i % 40 != 0 && i % 40 != 39) {
            sviVrhovi[i].susjedi.push_back(i - 1);
            sviVrhovi[i].susjedi.push_back(i + 1);
            sviVrhovi[i].susjedi.push_back(i - 40);
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] = (i == j) ? 0 : inf;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < sviVrhovi[i].susjedi.size(); j++) {
            mat[i][sviVrhovi[i].susjedi[j]] = 1;
        }
    }
}