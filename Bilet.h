#ifndef BILET_H
#define BILET_H

#include <iostream>
#include <string>
#include "Exceptii.h"

// ============================================================
//  Clasa Bilet  -  emis pentru un loc dintr-o proiectie
// ============================================================
class Bilet {
public:
    // tipuri de bilet cu reducerile asociate
    enum class Tip { Adult, Copil, Senior, Student };

    Bilet();
    Bilet(int nrLoc, Tip tip, double pretBaza);
    Bilet(const Bilet& other);
    ~Bilet() = default;

    Bilet& operator=(Bilet other);
    friend void swap(Bilet& a, Bilet& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Bilet& b);

    int         getNrLoc()  const { return nrLoc_; }
    Tip         getTip()    const { return tip_; }
    double      getPretBaza() const { return pretBaza_; }
    int         getId()     const { return id_; }

    double pretFinal()   const;
    double reducere()    const;
    void   afisareChitanta(const std::string& titluFilm,
                           int idSala,
                           const std::string& dataOra) const;

    static std::string tipToString(Tip t);
    static int         totalBileteEmise() { return contor_; }

private:
    int    id_;
    int    nrLoc_;
    Tip    tip_;
    double pretBaza_;

    static int contor_;
    static int urmatorulId_;

    double procentReducere() const;
};

#endif // BILET_H
