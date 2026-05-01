#ifndef FILM_H
#define FILM_H

#include <iostream>
#include <cstring>

/*
 * Clasa Film retine informatiile despre un film din cinematograf.
 * Atribute: titlu, gen, durata (minute), rating (0.0 - 10.0), an_aparitie
 */
class Film {
private:
    char* titlu;
    char* gen;
    int   durata;      // minute
    float rating;      // 0.0 - 10.0
    int   an_aparitie;

    static int contor; // numarul total de filme create

    // validare interna
    bool ratingValid(float r) const { return r >= 0.0f && r <= 10.0f; }
    bool durataValida(int d)  const { return d > 0; }

public:
    // Constructori
    Film();
    Film(const char* titlu, const char* gen, int durata, float rating, int an);
    Film(const Film& f);

    // Destructor
    ~Film();

    // Operator=
    Film& operator=(const Film& f);

    // Operator<< (friend pentru afisare)
    friend std::ostream& operator<<(std::ostream& out, const Film& f);

    // Getteri
    const char* getTitlu()      const { return titlu; }
    const char* getGen()        const { return gen; }
    int         getDurata()     const { return durata; }
    float       getRating()     const { return rating; }
    int         getAnAparitie() const { return an_aparitie; }

    // Setteri
    void setTitlu(const char* t);
    void setGen(const char* g);
    void setDurata(int d);
    void setRating(float r);
    void setAnAparitie(int a);

    // Metode functionale
    bool esteRecent(int an_curent) const;          // film aparut in ultimii 2 ani
    bool areRatingMinim(float min) const;          // verifica rating minim
    bool esteDeGen(const char* g) const;           // verifica gen

    static int getContor() { return contor; }
};

#endif // FILM_H
