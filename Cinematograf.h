#ifndef CINEMATOGRAF_H
#define CINEMATOGRAF_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Proiectie.h"
#include "Exceptii.h"

// ============================================================
//  Clasa Cinematograf
//  Contine un vector de shared_ptr<Proiectie> — apelam
//  functiile virtuale prin pointer de baza.
// ============================================================
class Cinematograf {
public:
    explicit Cinematograf(const std::string& nume, const std::string& adresa = "");
    Cinematograf(const Cinematograf& other);
    Cinematograf& operator=(Cinematograf other);
    ~Cinematograf() = default;

    friend void swap(Cinematograf& a, Cinematograf& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Cinematograf& c);

    // ── Adaugare entitati ────────────────────────────────────────────────
    // arunca EroareRezervare daca proiectia are ID duplicat
    void adaugaProiectie(const std::shared_ptr<Proiectie>& p);
    void adaugaSala(const Sala& s);

    // ── Acces proiectii ──────────────────────────────────────────────────
    // arunca EroareRezervare daca indexul e invalid
    Proiectie&       getProiectie(int index);
    const Proiectie& getProiectie(int index) const;
    int              nrProiectii() const { return static_cast<int>(proiectii_.size()); }

    // Afiseaza rezumatul tuturor proiectiilor
    void afiseazaToate() const;

    // ── Functii de nivel inalt (apeleaza virtuale prin pointer de baza) ──
    // Incasare totala (suma incasarilor reale ale tuturor proiectiilor)
    double incasareTotala() const;
    // Top N proiectii dupa incasare reala (sortare, fara modificarea vectorului original)
    void   topProiectii(int n) const;
    // Cauta proiectii dupa cuvant cheie in titlu
    void   cautaDupaTitlu(const std::string& cheie) const;
    // Afiseaza proiectii cu locuri disponibile
    void   proiectiiDisponibile() const;

    // ── Citire din fisier ────────────────────────────────────────────────
    // arunca EroareRezervare daca fisierul nu se poate deschide
    void incarcaDinFisier(const std::string& cale);

    static int totalCinematografe() { return contor_; }

private:
    std::string                             nume_;
    std::string                             adresa_;
    std::vector<std::shared_ptr<Proiectie>> proiectii_;
    std::vector<Sala>                       sali_;

    static int contor_;
};

#endif // CINEMATOGRAF_H
