#ifndef SALA_H
#define SALA_H

#include <iostream>
#include <string>
#include <vector>
#include "Exceptii.h"

// ============================================================
//  Clasa Sala  -  gestioneaza locurile dintr-o sala de cinema
// ============================================================
class Sala {
public:
    enum class Tip { Standard2D, Standard3D, IMAX };

    Sala();
    Sala(int id, const std::string& nume, int capacitate, Tip tip);
    Sala(const Sala& other)            = default;
    Sala& operator=(Sala other);
    ~Sala()                            = default;

    friend void swap(Sala& a, Sala& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Sala& s);

    int               getId()         const { return id_; }
    const std::string& getNume()      const { return nume_; }
    int               getCapacitate() const { return static_cast<int>(locuri_.size()); }
    Tip               getTip()        const { return tip_; }

    // arunca EroareSala daca locul e invalid sau deja ocupat
    void  ocupaLoc(int idx);
    void  elibereazaLoc(int idx);
    bool  esteDisponibil(int idx) const;
    int   locuriDisponibile() const;
    float procentOcupare()    const;
    void  afiseazaHarta()     const;
    void  resetLocuri();

    static std::string tipToString(Tip t);
    static int         totalSali() { return contor_; }

private:
    int               id_;
    std::string       nume_;
    std::vector<bool> locuri_;   // true = ocupat
    Tip               tip_;

    static int contor_;

    void verificaIndex(int idx) const;
};

#endif // SALA_H
