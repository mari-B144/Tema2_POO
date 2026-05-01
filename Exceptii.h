#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <stdexcept>
#include <string>

// ============================================================
//  Ierarhie proprie de exceptii, baza derivata din std::runtime_error
// ============================================================

class EroareCinematograf : public std::runtime_error {
public:
    explicit EroareCinematograf(const std::string& mesaj)
        : std::runtime_error("[Cinematograf] " + mesaj) {}
};

// Eroare la validarea unui bilet (tip invalid, pret negativ etc.)
class EroareBilet : public EroareCinematograf {
public:
    explicit EroareBilet(const std::string& mesaj)
        : EroareCinematograf("[Bilet] " + mesaj) {}
};

// Eroare la sala (loc deja ocupat, index invalid, sala plina)
class EroareSala : public EroareCinematograf {
public:
    explicit EroareSala(const std::string& mesaj)
        : EroareCinematograf("[Sala] " + mesaj) {}
};

// Eroare la crearea/gasirea unei rezervari sau proiectii
class EroareRezervare : public EroareCinematograf {
public:
    explicit EroareRezervare(const std::string& mesaj)
        : EroareCinematograf("[Rezervare] " + mesaj) {}
};

#endif // EXCEPTII_H
