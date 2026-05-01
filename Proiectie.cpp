#include "Proiectie.h"
#include <iomanip>
#include <algorithm>
#include <numeric>

// ── Proiectie (baza) ─────────────────────────────────────────────────────────

int Proiectie::contor_      = 0;
int Proiectie::urmatorulId_ = 1;

Proiectie::Proiectie(const std::string& titlu,
                     const Sala& sala,
                     const std::string& dataOra)
    : id_(urmatorulId_++), titluFilm_(titlu), sala_(sala), dataOra_(dataOra)
{
    if (titlu.empty())
        throw EroareRezervare("titlul filmului nu poate fi gol");
    if (dataOra.empty())
        throw EroareRezervare("data/ora proiectiei nu poate fi goala");
    ++contor_;
}

Proiectie::Proiectie(const Proiectie& other)
    : id_(urmatorulId_++),
      titluFilm_(other.titluFilm_),
      sala_(other.sala_),
      dataOra_(other.dataOra_),
      bilete_(other.bilete_)
{ ++contor_; }

Proiectie& Proiectie::operator=(const Proiectie& other) {
    if (this != &other) {
        titluFilm_ = other.titluFilm_;
        sala_      = other.sala_;
        dataOra_   = other.dataOra_;
        bilete_    = other.bilete_;
    }
    return *this;
}

void swap(Proiectie& a, Proiectie& b) noexcept {
    using std::swap;
    swap(a.titluFilm_, b.titluFilm_);
    swap(a.sala_,      b.sala_);
    swap(a.dataOra_,   b.dataOra_);
    swap(a.bilete_,    b.bilete_);
    // id_ isi pastreaza valoarea
}

std::ostream& operator<<(std::ostream& os, const Proiectie& p) {
    os << "[Proiectie #" << p.id_ << "] \""
       << p.titluFilm_ << "\""
       << " | " << p.dataOra_
       << " | Sala:" << p.sala_.getId()
       << " | Bilete:" << p.getNrBilete()
       << " | Incasare:" << std::fixed << std::setprecision(2)
       << p.incasareReala() << " RON"
       << " | " << p.descriere();
    p.afisare_(os);
    return os;
}

// ── NVI: implementari publice ────────────────────────────────────────────────

double Proiectie::incasareEstimata() const {
    return sala_.getCapacitate() * pretBilet_();
}

double Proiectie::incasareReala() const {
    double total = 0.0;
    for (const auto& b : bilete_) total += b.pretFinal();
    return total;
}

void Proiectie::adaugaBilet(int nrLoc, Bilet::Tip tip) {
    // arunca EroareSala daca locul e invalid/ocupat
    sala_.ocupaLoc(nrLoc);
    // arunca EroareBilet daca pretul e invalid (nu poate fi 0 sau negativ)
    Bilet b(nrLoc, tip, pretBilet_());
    bilete_.push_back(std::move(b));
    std::cout << "  [OK] Loc " << nrLoc + 1
              << " | " << Bilet::tipToString(tip)
              << " | " << std::fixed << std::setprecision(2)
              << bilete_.back().pretFinal() << " RON\n";
}

bool Proiectie::anuleazaBilet(int idBilet) {
    auto it = std::find_if(bilete_.begin(), bilete_.end(),
                           [idBilet](const Bilet& b){ return b.getId() == idBilet; });
    if (it == bilete_.end()) {
        std::cout << "  [EROARE] Bilet #" << idBilet << " negasit.\n";
        return false;
    }
    sala_.elibereazaLoc(it->getNrLoc());
    bilete_.erase(it);
    std::cout << "  [OK] Bilet #" << idBilet << " anulat.\n";
    return true;
}

void Proiectie::afisareChitanta(int idBilet) const {
    auto it = std::find_if(bilete_.begin(), bilete_.end(),
                           [idBilet](const Bilet& b){ return b.getId() == idBilet; });
    if (it == bilete_.end()) {
        std::cout << "  [EROARE] Bilet #" << idBilet << " negasit.\n";
        return;
    }
    it->afisareChitanta(titluFilm_, sala_.getId(), dataOra_);
}

void Proiectie::afisareRaport() const {
    std::cout << "\n============================================\n";
    std::cout << "  RAPORT PROIECTIE #" << id_ << "\n";
    std::cout << "============================================\n";
    std::cout << "  Film    : " << titluFilm_ << "\n";
    std::cout << "  Tip     : " << descriere_() << "\n";
    std::cout << "  Data    : " << dataOra_ << "\n";
    std::cout << "  " << sala_ << "\n";
    std::cout << "  Pret/loc: " << std::fixed << std::setprecision(2)
              << pretBilet_() << " RON\n";
    std::cout << "  Estimat : " << incasareEstimata() << " RON\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  Bilete vandute (" << bilete_.size() << "):\n";
    for (const auto& b : bilete_)
        std::cout << "    " << b << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  INCASARE REALA: " << incasareReala() << " RON\n";
    std::cout << "============================================\n";
}

// Implementare default afisare_ — derivatele o pot suprascrie
void Proiectie::afisare_(std::ostream&) const {}

// ── ProiectieStandard ────────────────────────────────────────────────────────

ProiectieStandard::ProiectieStandard(const std::string& titlu,
                                     const Sala& sala,
                                     const std::string& dataOra,
                                     double pretBaza)
    : Proiectie(titlu, sala, dataOra), pretBaza_(pretBaza)
{
    if (pretBaza <= 0.0)
        throw EroareBilet("pret baza invalid pentru ProiectieStandard: "
                          + std::to_string(pretBaza));
}

ProiectieStandard::ProiectieStandard(const ProiectieStandard& other)
    : Proiectie(other), pretBaza_(other.pretBaza_) {}

ProiectieStandard& ProiectieStandard::operator=(ProiectieStandard other) {
    static_cast<Proiectie&>(*this) = other;
    using std::swap;
    swap(pretBaza_, other.pretBaza_);
    return *this;
}

std::shared_ptr<Proiectie> ProiectieStandard::clone() const {
    return std::make_shared<ProiectieStandard>(*this);
}

std::string ProiectieStandard::descriere_() const {
    return "Standard (" + Sala::tipToString(getSala().getTip()) + ")";
}

void ProiectieStandard::afisare_(std::ostream& os) const {
    os << " Pret:" << std::fixed << std::setprecision(2) << pretBaza_ << " RON";
}

// ── ProiectieIMAX ────────────────────────────────────────────────────────────

ProiectieIMAX::ProiectieIMAX(const std::string& titlu,
                              const Sala& sala,
                              const std::string& dataOra,
                              double pretBaza,
                              double multiplicator)
    : Proiectie(titlu, sala, dataOra),
      pretBaza_(pretBaza),
      multiplicator_(multiplicator)
{
    if (pretBaza <= 0.0 || multiplicator <= 0.0)
        throw EroareBilet("parametri invalizi pentru ProiectieIMAX");
}

ProiectieIMAX::ProiectieIMAX(const ProiectieIMAX& other)
    : Proiectie(other),
      pretBaza_(other.pretBaza_),
      multiplicator_(other.multiplicator_) {}

ProiectieIMAX& ProiectieIMAX::operator=(ProiectieIMAX other) {
    static_cast<Proiectie&>(*this) = other;
    using std::swap;
    swap(pretBaza_,      other.pretBaza_);
    swap(multiplicator_, other.multiplicator_);
    return *this;
}

std::shared_ptr<Proiectie> ProiectieIMAX::clone() const {
    return std::make_shared<ProiectieIMAX>(*this);
}

std::string ProiectieIMAX::descriere_() const {
    return "IMAX (x" + std::to_string(multiplicator_) + ")";
}

void ProiectieIMAX::afisare_(std::ostream& os) const {
    os << " Pret:" << std::fixed << std::setprecision(2)
       << pretBaza_ << "*" << multiplicator_
       << "=" << pretBilet_() << " RON";
}

// ── ProiectieSpeciala ────────────────────────────────────────────────────────

ProiectieSpeciala::ProiectieSpeciala(const std::string& titlu,
                                     const Sala& sala,
                                     const std::string& dataOra,
                                     double pretPremium,
                                     TipEveniment eveniment)
    : Proiectie(titlu, sala, dataOra),
      pretPremium_(pretPremium),
      eveniment_(eveniment)
{
    if (pretPremium <= 0.0)
        throw EroareBilet("pret premium invalid pentru ProiectieSpeciala: "
                          + std::to_string(pretPremium));
}

ProiectieSpeciala::ProiectieSpeciala(const ProiectieSpeciala& other)
    : Proiectie(other),
      pretPremium_(other.pretPremium_),
      eveniment_(other.eveniment_) {}

ProiectieSpeciala& ProiectieSpeciala::operator=(ProiectieSpeciala other) {
    static_cast<Proiectie&>(*this) = other;
    using std::swap;
    swap(pretPremium_, other.pretPremium_);
    swap(eveniment_,   other.eveniment_);
    return *this;
}

std::shared_ptr<Proiectie> ProiectieSpeciala::clone() const {
    return std::make_shared<ProiectieSpeciala>(*this);
}

std::string ProiectieSpeciala::evenimentToString(TipEveniment t) {
    switch (t) {
        case TipEveniment::Premiera:      return "Premiera";
        case TipEveniment::Festival:      return "Festival";
        case TipEveniment::Retrospectiva: return "Retrospectiva";
        default:                          return "Eveniment";
    }
}

std::string ProiectieSpeciala::descriere_() const {
    return evenimentToString(eveniment_) + " (premium)";
}

void ProiectieSpeciala::afisare_(std::ostream& os) const {
    os << " Eveniment:" << evenimentToString(eveniment_)
       << " Pret:" << std::fixed << std::setprecision(2)
       << pretPremium_ << " RON";
}
