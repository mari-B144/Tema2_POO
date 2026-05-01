#include "Bilet.h"
#include <iomanip>

int Bilet::contor_      = 0;
int Bilet::urmatorulId_ = 1;

Bilet::Bilet()
    : id_(urmatorulId_++), nrLoc_(0), tip_(Tip::Adult), pretBaza_(30.0)
{
    ++contor_;
}

Bilet::Bilet(int nrLoc, Tip tip, double pretBaza)
    : id_(urmatorulId_++), nrLoc_(nrLoc), tip_(tip), pretBaza_(pretBaza)
{
    if (pretBaza <= 0.0)
        throw EroareBilet("pretul de baza trebuie sa fie pozitiv, primit: "
                          + std::to_string(pretBaza));
    if (nrLoc < 0)
        throw EroareBilet("numarul locului nu poate fi negativ, primit: "
                          + std::to_string(nrLoc));
    ++contor_;
}

Bilet::Bilet(const Bilet& other)
    : id_(urmatorulId_++),   // bilet nou => ID nou
      nrLoc_(other.nrLoc_),
      tip_(other.tip_),
      pretBaza_(other.pretBaza_)
{
    ++contor_;
}

Bilet& Bilet::operator=(Bilet other) {
    swap(*this, other);
    return *this;
}

void swap(Bilet& a, Bilet& b) noexcept {
    using std::swap;
    swap(a.nrLoc_,   b.nrLoc_);
    swap(a.tip_,     b.tip_);
    swap(a.pretBaza_,b.pretBaza_);
    // id_ nu se schimba — fiecare bilet isi pastreaza identitatea
}

std::ostream& operator<<(std::ostream& os, const Bilet& b) {
    os << "[Bilet #" << b.id_ << "]"
       << " Loc:" << b.nrLoc_ + 1
       << " Tip:" << Bilet::tipToString(b.tip_)
       << " Baza:" << std::fixed << std::setprecision(2) << b.pretBaza_ << " RON"
       << " Final:" << b.pretFinal() << " RON";
    if (b.reducere() > 0.0)
        os << " (-" << (int)(b.procentReducere() * 100) << "%)";
    return os;
}

double Bilet::procentReducere() const {
    switch (tip_) {
        case Tip::Copil:   return 0.30;
        case Tip::Senior:  return 0.20;
        case Tip::Student: return 0.15;
        default:           return 0.00;
    }
}

double Bilet::pretFinal()  const { return pretBaza_ * (1.0 - procentReducere()); }
double Bilet::reducere()   const { return pretBaza_ * procentReducere(); }

void Bilet::afisareChitanta(const std::string& titlu,
                             int idSala,
                             const std::string& dataOra) const {
    std::cout << "\n  +--------------------------------------+\n";
    std::cout <<   "  |        CINEMATOGRAF  -  BILET        |\n";
    std::cout <<   "  +--------------------------------------+\n";
    std::cout <<   "  | Bilet #" << id_ << "\n";
    std::cout <<   "  | Film  : " << titlu << "\n";
    std::cout <<   "  | Data  : " << dataOra << "\n";
    std::cout <<   "  | Sala  : " << idSala << "\n";
    std::cout <<   "  | Loc   : " << nrLoc_ + 1 << "\n";
    std::cout <<   "  | Tip   : " << tipToString(tip_) << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout <<   "  | Baza  : " << pretBaza_ << " RON\n";
    if (reducere() > 0.0)
        std::cout << "  | Reduc.: -" << reducere()
                  << " RON (" << (int)(procentReducere()*100) << "%)\n";
    std::cout <<   "  +--------------------------------------+\n";
    std::cout <<   "  | TOTAL : " << pretFinal() << " RON\n";
    std::cout <<   "  +--------------------------------------+\n";
}

std::string Bilet::tipToString(Tip t) {
    switch (t) {
        case Tip::Adult:   return "adult";
        case Tip::Copil:   return "copil";
        case Tip::Senior:  return "senior";
        case Tip::Student: return "student";
        default:           return "necunoscut";
    }
}
