/*
 * ============================================================
 * PROIECT POO  -  Sistem Management Cinematograf (Tema 2)
 * ============================================================
 *
 * DESCRIEREA PROBLEMEI:
 * Sistem de gestiune pentru un cinematograf cu ierarhie de
 * proiectii, exceptii proprii si meniu interactiv.
 *
 * Date de intrare (date.txt):
 *   [SALI]       : id|nume|capacitate|tip
 *   [PROIECTII]  : tip|titlu|id_sala|data_ora|pret_baza[|extra]
 *   [BILETE]     : id_proiectie|nr_loc|tip_bilet
 *
 * IERARHIE PROIECTIE (clasa abstracta + 3 derivate):
 *   Proiectie (abstracta, NVI)
 *     |- ProiectieStandard  : proiectii 2D/3D normale
 *     |- ProiectieIMAX      : proiectii cu multiplicator pret
 *     |- ProiectieSpeciala  : premiere, festival, retrospectiva
 *     |- ProiectieVIP       : (comit separat) sala VIP, open bar
 *
 * IERARHIE EXCEPTII:
 *   std::runtime_error
 *     |- EroareCinematograf (baza proprie)
 *          |- EroareBilet
 *          |- EroareSala
 *          |- EroareRezervare
 *
 * MENIU INTERACTIV:
 *   1. Afisare toate proiectiile
 *   2. Raport detaliat proiectie
 *   3. Harta locuri sala
 *   4. Vanzare bilet
 *   5. Anulare bilet
 *   6. Chitanta bilet
 *   7. Cautare dupa titlu
 *   8. Top proiectii dupa incasare
 *   9. Incasare totala
 *   0. Iesire
 * ============================================================
 */

#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <iomanip>
#include "Cinematograf.h"
#include "Proiectie.h"
#include "ProiectieVIP.h"
#include "Exceptii.h"

// ── utilitare I/O ─────────────────────────────────────────────────────────────

static void linie(char c = '-', int n = 52) {
    for (int i = 0; i < n; ++i) std::cout << c;
    std::cout << "\n";
}

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(1000, '\n');
}

static int citesteInt(const std::string& prompt) {
    int val;
    std::cout << prompt;
    while (!(std::cin >> val)) { clearInput(); std::cout << "  Numar intreg: "; }
    clearInput();
    return val;
}

static std::string citesteStr(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

static void afiseazaMeniu() {
    std::cout << "\n";
    linie('=');
    std::cout << "       MENIU  -  CineMax Bucuresti\n";
    linie('=');
    std::cout << "  1  Afisare toate proiectiile\n";
    std::cout << "  2  Raport detaliat proiectie\n";
    std::cout << "  3  Harta locuri sala\n";
    std::cout << "  4  Vanzare bilet\n";
    std::cout << "  5  Anulare bilet\n";
    std::cout << "  6  Chitanta bilet\n";
    std::cout << "  7  Cautare dupa titlu\n";
    std::cout << "  8  Top proiectii dupa incasare\n";
    std::cout << "  9  Incasare totala\n";
    std::cout << "  0  Iesire\n";
    linie('-');
    std::cout << "  Alegeti optiunea: ";
}

// cere un index valid de proiectie din cinematograf
static int citesteIndexProiectie(const Cinematograf& c) {
    int nr = c.nrProiectii();
    if (nr == 0) { std::cout << "  (nicio proiectie)\n"; return -1; }
    c.afiseazaToate();
    int idx = citesteInt("  Selectati indexul proiectiei: ");
    if (idx < 0 || idx >= nr) {
        std::cout << "  [!] Index invalid (0 - " << nr - 1 << ")\n";
        return -1;
    }
    return idx;
}

// ── main ──────────────────────────────────────────────────────────────────────

int main() {
    linie('*');
    std::cout << "   SISTEM MANAGEMENT CINEMATOGRAF  -  Tema 2 POO\n";
    linie('*');

    Cinematograf cinema("CineMax Bucuresti", "Bd. Unirii nr. 10");

    // ── incarcare date si demonstrare throw in constructor ────────────────────
    try {
        cinema.incarcaDinFisier("date.txt");
    } catch (const EroareRezervare& e) {
        std::cout << "[FATAL] " << e.what() << "\n";
        return 1;
    }

    // ── demonstrare ierarhie exceptii ─────────────────────────────────────────
    std::cout << "\n>>> Demonstrare exceptii:\n";
    try {
        // EroareBilet — pret negativ in constructor Bilet
        Bilet bGresit(0, Bilet::Tip::Adult, -10.0);
    } catch (const EroareBilet& e) {
        std::cout << "  Prins EroareBilet: " << e.what() << "\n";
    }
    try {
        // EroareSala — capacitate 0 in constructor Sala
        Sala sGresita(99, "Test", 0, Sala::Tip::Standard2D);
    } catch (const EroareSala& e) {
        std::cout << "  Prins EroareSala: " << e.what() << "\n";
    }
    try {
        // EroareRezervare — fisier inexistent
        cinema.incarcaDinFisier("inexistent.txt");
    } catch (const EroareRezervare& e) {
        std::cout << "  Prins EroareRezervare: " << e.what() << "\n";
    }
    try {
        // Prindere prin baza EroareCinematograf
        Bilet bGresit2(0, Bilet::Tip::Copil, -5.0);
    } catch (const EroareCinematograf& e) {
        std::cout << "  Prins prin baza EroareCinematograf: " << e.what() << "\n";
    }

    // ── demonstrare clone / cc / op= ──────────────────────────────────────────
    std::cout << "\n>>> Demonstrare clone / cc / op=:\n";
    {
        Sala s1(10, "Sala VIP", 15, Sala::Tip::IMAX);
        ProiectieIMAX p1("Gladiator II", s1, "29/03/2025 21:00", 50.0, 1.7);
        auto p2 = p1.clone();                    // constructor virtual
        ProiectieIMAX p3(p1);                    // constructor de copiere
        p3.adaugaBilet(0, Bilet::Tip::Adult);
        std::cout << "  Original : " << p1 << "\n";
        std::cout << "  Clone    : " << *p2 << "\n";
        std::cout << "  CC+bilet : " << p3 << "\n";
        // dynamic_cast: downcast de la baza la derivata
        if (auto* imax = dynamic_cast<ProiectieIMAX*>(p2.get())) {
            std::cout << "  dynamic_cast reusit -> " << imax->descriere() << "\n";
        }
        if (dynamic_cast<ProiectieStandard*>(p2.get()) == nullptr) {
            std::cout << "  dynamic_cast la tip gresit -> nullptr (corect)\n";
        }
    }

    // ── demonstrare ProiectieVIP (a 4-a derivata) ───────────────────────────────
    std::cout << "\n>>> Demonstrare ProiectieVIP:\n";
    {
        Sala sVip(20, "Sala VIP", 10, Sala::Tip::Standard2D);
        ProiectieVIP pVip("La La Land", sVip, "30/03/2025 20:00", 45.0, 35.0);
        pVip.adaugaBilet(0, Bilet::Tip::Adult);
        pVip.adaugaBilet(1, Bilet::Tip::Student);
        std::cout << "  " << pVip << "\n";
        // clone si dynamic_cast
        auto clona = pVip.clone();
        if (auto* vip = dynamic_cast<ProiectieVIP*>(clona.get()))
            std::cout << "  dynamic_cast la ProiectieVIP reusit: " << vip->descriere() << "\n";
        cinema.adaugaProiectie(pVip.clone());
        std::cout << "  Adaugata la cinematograf. Total proiectii: "
                  << cinema.nrProiectii() << "\n";
    }

    // ── demonstrare static ────────────────────────────────────────────────────
    std::cout << "\n>>> Contoare statice:\n";
    std::cout << "  Bilete emise total : " << Bilet::totalBileteEmise() << "\n";
    std::cout << "  Sali create total  : " << Sala::totalSali() << "\n";
    std::cout << "  Proiectii create   : " << Proiectie::totalProiectii() << "\n";
    std::cout << "  Cinematografe      : " << Cinematograf::totalCinematografe() << "\n";

    // ── operator= Cinematograf (copy and swap) ───────────────────────────────
    std::cout << "\n>>> Test operator= Cinematograf:\n";
    Cinematograf cinema2 = cinema;
    std::cout << cinema2 << "\n";

    // ── meniu interactiv ─────────────────────────────────────────────────────
    int optiune = -1;
    while (true) {
        afiseazaMeniu();
        std::cin >> optiune;
        clearInput();
        std::cout << "\n";

        try {
            switch (optiune) {

            case 0:
                std::cout << "  La revedere!\n";
                return 0;

            case 1:
                linie(); std::cout << "  TOATE PROIECTIILE (" << cinema.nrProiectii() << ")\n"; linie();
                cinema.afiseazaToate();
                break;

            case 2: {
                linie(); std::cout << "  RAPORT DETALIAT\n"; linie();
                int idx = citesteIndexProiectie(cinema);
                if (idx >= 0) cinema.getProiectie(idx).afisareRaport();
                break;
            }

            case 3: {
                linie(); std::cout << "  HARTA LOCURI\n"; linie();
                int idx = citesteIndexProiectie(cinema);
                if (idx >= 0) {
                    std::cout << "  " << cinema.getProiectie(idx).getSala() << "\n";
                    cinema.getProiectie(idx).afisareHartaSala();
                }
                break;
            }

            case 4: {
                linie(); std::cout << "  VANZARE BILET\n"; linie();
                int idx = citesteIndexProiectie(cinema);
                if (idx < 0) break;
                Proiectie& p = cinema.getProiectie(idx);
                if (!p.areLocuriDisponibile()) {
                    std::cout << "  [!] Sala este plina!\n"; break;
                }
                p.afisareHartaSala();
                std::cout << "  Locuri disponibile: " << p.getSala().locuriDisponibile()
                          << "/" << p.getSala().getCapacitate() << "\n";
                int loc = citesteInt("  Numarul locului (1-based): ") - 1;
                std::cout << "  Tip bilet [1=adult 2=copil 3=senior 4=student]: ";
                int tipInt; std::cin >> tipInt; clearInput();
                Bilet::Tip tip = Bilet::Tip::Adult;
                if (tipInt == 2) tip = Bilet::Tip::Copil;
                if (tipInt == 3) tip = Bilet::Tip::Senior;
                if (tipInt == 4) tip = Bilet::Tip::Student;
                // poate arunca EroareSala sau EroareBilet
                p.adaugaBilet(loc, tip);
                break;
            }

            case 5: {
                linie(); std::cout << "  ANULARE BILET\n"; linie();
                int idx = citesteIndexProiectie(cinema);
                if (idx < 0) break;
                Proiectie& p = cinema.getProiectie(idx);
                if (p.getNrBilete() == 0) { std::cout << "  (niciun bilet)\n"; break; }
                p.afisareRaport();
                int idB = citesteInt("  ID bilet de anulat: ");
                p.anuleazaBilet(idB);
                break;
            }

            case 6: {
                linie(); std::cout << "  CHITANTA BILET\n"; linie();
                int idx = citesteIndexProiectie(cinema);
                if (idx < 0) break;
                Proiectie& p = cinema.getProiectie(idx);
                if (p.getNrBilete() == 0) { std::cout << "  (niciun bilet)\n"; break; }
                p.afisareRaport();
                int idB = citesteInt("  ID bilet: ");
                p.afisareChitanta(idB);
                break;
            }

            case 7: {
                linie(); std::cout << "  CAUTARE DUPA TITLU\n"; linie();
                std::string cheie = citesteStr("  Cuvant cheie: ");
                cinema.cautaDupaTitlu(cheie);
                break;
            }

            case 8: {
                linie(); std::cout << "  TOP PROIECTII\n"; linie();
                int n = citesteInt("  Cate pozitii: ");
                cinema.topProiectii(n);
                break;
            }

            case 9:
                linie(); std::cout << "  INCASARE TOTALA\n"; linie();
                std::cout << "  Total incasat: " << std::fixed << std::setprecision(2)
                          << cinema.incasareTotala() << " RON\n";
                break;

            default:
                std::cout << "  [!] Optiune invalida (0-9)\n";
            }

        // prindere exceptii din operatii interactive
        } catch (const EroareSala& e) {
            std::cout << "  [EROARE SALA] " << e.what() << "\n";
        } catch (const EroareBilet& e) {
            std::cout << "  [EROARE BILET] " << e.what() << "\n";
        } catch (const EroareRezervare& e) {
            std::cout << "  [EROARE REZERVARE] " << e.what() << "\n";
        } catch (const EroareCinematograf& e) {
            std::cout << "  [EROARE] " << e.what() << "\n";
        }
    }

    return 0;
}
