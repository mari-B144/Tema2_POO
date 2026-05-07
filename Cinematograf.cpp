#include "Cinematograf.h"
#include "ProiectieVIP.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

int Cinematograf::contor_ = 0;

Cinematograf::Cinematograf(const std::string& nume, const std::string& adresa)
    : nume_(nume), adresa_(adresa)
{ ++contor_; }

Cinematograf::Cinematograf(const Cinematograf& other)
    : nume_(other.nume_), adresa_(other.adresa_), sali_(other.sali_)
{
    for (const auto& p : other.proiectii_)
        proiectii_.push_back(p->clone());
    ++contor_;
}

Cinematograf& Cinematograf::operator=(Cinematograf other) {
    swap(*this, other);
    return *this;
}

void swap(Cinematograf& a, Cinematograf& b) noexcept {
    using std::swap;
    swap(a.nume_,      b.nume_);
    swap(a.adresa_,    b.adresa_);
    swap(a.proiectii_, b.proiectii_);
    swap(a.sali_,      b.sali_);
}

std::ostream& operator<<(std::ostream& os, const Cinematograf& c) {
    os << "=== " << c.nume_;
    if (!c.adresa_.empty()) os << " | " << c.adresa_;
    os << " ===\n"
       << "  Proiectii: " << c.nrProiectii()
       << " | Incasare totala: " << std::fixed << std::setprecision(2)
       << c.incasareTotala() << " RON";
    return os;
}

void Cinematograf::adaugaProiectie(const std::shared_ptr<Proiectie>& p) {
    proiectii_.push_back(p);
}

void Cinematograf::adaugaSala(const Sala& s) {
    sali_.push_back(s);
}

Proiectie& Cinematograf::getProiectie(int index) {
    if (index < 0 || index >= static_cast<int>(proiectii_.size()))
        throw EroareRezervare("index proiectie invalid: " + std::to_string(index));
    return *proiectii_[index];
}

const Proiectie& Cinematograf::getProiectie(int index) const {
    if (index < 0 || index >= static_cast<int>(proiectii_.size()))
        throw EroareRezervare("index proiectie invalid: " + std::to_string(index));
    return *proiectii_[index];
}

void Cinematograf::afiseazaToate() const {
    if (proiectii_.empty()) {
        std::cout << "  (nicio proiectie)\n";
        return;
    }
    for (int i = 0; i < static_cast<int>(proiectii_.size()); ++i)
        std::cout << "  [" << i << "] " << *proiectii_[i] << "\n";
}

// ── Functii de nivel inalt ────────────────────────────────────────────────────

double Cinematograf::incasareTotala() const {
    double total = 0.0;
    // apel prin pointer de baza — incasareReala() foloseste pretBilet_() virtual
    for (const auto& p : proiectii_)
        total += p->incasareReala();
    return total;
}

void Cinematograf::topProiectii(int n) const {
    if (proiectii_.empty()) { std::cout << "  (nicio proiectie)\n"; return; }
    // copie de pointeri pentru sortare fara modificarea vectorului original
    std::vector<std::shared_ptr<Proiectie>> copie(proiectii_);
    std::sort(copie.begin(), copie.end(),
              [](const auto& a, const auto& b){
                  return a->incasareReala() > b->incasareReala();
              });
    int afis = std::min(n, static_cast<int>(copie.size()));
    std::cout << "\n  Top " << afis << " proiectii dupa incasare:\n";
    for (int i = 0; i < afis; ++i) {
        std::cout << "  " << (i+1) << ". " << copie[i]->getTitluFilm()
                  << " | " << copie[i]->descriere()
                  << " | " << std::fixed << std::setprecision(2)
                  << copie[i]->incasareReala() << " RON\n";
    }
}

void Cinematograf::cautaDupaTitlu(const std::string& cheie) const {
    std::cout << "  Rezultate pentru \"" << cheie << "\":\n";
    bool gasit = false;
    for (const auto& p : proiectii_) {
        if (p->getTitluFilm().find(cheie) != std::string::npos) {
            std::cout << "  -> " << *p << "\n";
            gasit = true;
        }
    }
    if (!gasit) std::cout << "  (niciun rezultat)\n";
}

void Cinematograf::proiectiiDisponibile() const {
    std::cout << "  Proiectii cu locuri disponibile:\n";
    bool gasit = false;
    for (int i = 0; i < static_cast<int>(proiectii_.size()); ++i) {
        if (proiectii_[i]->areLocuriDisponibile()) {
            std::cout << "  [" << i << "] " << *proiectii_[i] << "\n";
            gasit = true;
        }
    }
    if (!gasit) std::cout << "  (nicio proiectie disponibila)\n";
}

// ── Citire din fisier ──────────────────────────────────────────────────────────
/*
 * FORMAT date.txt:
 *
 * [SALI]
 * id|nume|capacitate|tip   (tip: 2D / 3D / IMAX)
 *
 * [PROIECTII]
 * tip|titlu|id_sala|data_ora|pret_baza[|extra]
 *   tip: standard / imax / speciala
 *   extra pentru imax: multiplicator
 *   extra pentru speciala: premiera / festival / retrospectiva
 *
 * [BILETE]
 * id_proiectie(index)|nr_loc(0-based)|tip_bilet
 *
 * Liniile cu # sunt comentarii.
 */
static std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string tok;
    while (std::getline(ss, tok, delim))
        tokens.push_back(tok);
    return tokens;
}

void Cinematograf::incarcaDinFisier(const std::string& cale) {
    std::ifstream fin(cale);
    if (!fin.is_open())
        throw EroareRezervare("nu pot deschide fisierul: " + cale);

    enum class Sectiune { NIMIC, SALI, PROIECTII, BILETE } sec = Sectiune::NIMIC;
    std::string linie;
    int nrSali = 0, nrProiectii = 0, nrBilete = 0;

    while (std::getline(fin, linie)) {
        // strip \r
        if (!linie.empty() && linie.back() == '\r') linie.pop_back();
        if (linie.empty() || linie[0] == '#') continue;

        if (linie == "[SALI]")       { sec = Sectiune::SALI;       continue; }
        if (linie == "[PROIECTII]")  { sec = Sectiune::PROIECTII;  continue; }
        if (linie == "[BILETE]")     { sec = Sectiune::BILETE;     continue; }

        auto tok = split(linie, '|');

        if (sec == Sectiune::SALI && tok.size() >= 4) {
            int id  = std::stoi(tok[0]);
            const std::string& nm = tok[1];
            int cap = std::stoi(tok[2]);
            Sala::Tip tip = Sala::Tip::Standard2D;
            if (tok[3] == "3D")   tip = Sala::Tip::Standard3D;
            if (tok[3] == "IMAX") tip = Sala::Tip::IMAX;
            adaugaSala(Sala(id, nm, cap, tip));
            ++nrSali;
        }
        else if (sec == Sectiune::PROIECTII && tok.size() >= 5) {
            const std::string& tipS   = tok[0];
            const std::string& titlu  = tok[1];
            int idSala                = std::stoi(tok[2]);
            const std::string& data   = tok[3];
            double pret               = std::stod(tok[4]);

            // cauta sala dupa ID
            auto it = std::find_if(sali_.begin(), sali_.end(),
                                   [idSala](const Sala& s){ return s.getId() == idSala; });
            if (it == sali_.end()) {
                std::cout << "  [ATENTIE] Sala " << idSala << " negasita, proiectie ignorata.\n";
                continue;
            }

            if (tipS == "standard") {
                adaugaProiectie(std::make_shared<ProiectieStandard>(titlu, *it, data, pret));
            } else if (tipS == "imax") {
                double mult = tok.size() >= 6 ? std::stod(tok[5]) : 1.5;
                adaugaProiectie(std::make_shared<ProiectieIMAX>(titlu, *it, data, pret, mult));
            } else if (tipS == "vip") {
                double taxa = tok.size() >= 6 ? std::stod(tok[5]) : 30.0;
                adaugaProiectie(std::make_shared<ProiectieVIP>(titlu, *it, data, pret, taxa));
            } else if (tipS == "speciala") {
                auto ev = ProiectieSpeciala::TipEveniment::Premiera;
                if (tok.size() >= 6) {
                    if (tok[5] == "festival")      ev = ProiectieSpeciala::TipEveniment::Festival;
                    if (tok[5] == "retrospectiva") ev = ProiectieSpeciala::TipEveniment::Retrospectiva;
                }
                adaugaProiectie(std::make_shared<ProiectieSpeciala>(titlu, *it, data, pret, ev));
            }
            ++nrProiectii;
        }
        else if (sec == Sectiune::BILETE && tok.size() >= 3) {
            int idx    = std::stoi(tok[0]);
            int nrLoc  = std::stoi(tok[1]);
            const std::string& tipS = tok[2];

            if (idx < 0 || idx >= static_cast<int>(proiectii_.size())) continue;

            Bilet::Tip tip = Bilet::Tip::Adult;
            if (tipS == "copil")   tip = Bilet::Tip::Copil;
            if (tipS == "senior")  tip = Bilet::Tip::Senior;
            if (tipS == "student") tip = Bilet::Tip::Student;

            try {
                proiectii_[idx]->adaugaBilet(nrLoc, tip);
                ++nrBilete;
            } catch (const EroareCinematograf& e) {
                std::cout << "  [ATENTIE] " << e.what() << "\n";
            }
        }
    }
    std::cout << "[OK] Incarcat: " << nrSali << " sali, "
              << nrProiectii << " proiectii, "
              << nrBilete    << " bilete din '" << cale << "'\n";
}