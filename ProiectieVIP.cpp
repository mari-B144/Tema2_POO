#include "ProiectieVIP.h"
#include <iomanip>

ProiectieVIP::ProiectieVIP(const std::string& titlu,
                           const Sala& sala,
                           const std::string& dataOra,
                           double pretBaza,
                           double taxaServicii)
    : Proiectie(titlu, sala, dataOra),
      pretBaza_(pretBaza),
      taxaServicii_(taxaServicii)
{
    if (pretBaza <= 0.0 || taxaServicii < 0.0)
        throw EroareBilet("parametri invalizi pentru ProiectieVIP");
}

ProiectieVIP::ProiectieVIP(const ProiectieVIP& other)
    : Proiectie(other),
      pretBaza_(other.pretBaza_),
      taxaServicii_(other.taxaServicii_) {}

ProiectieVIP& ProiectieVIP::operator=(ProiectieVIP other) {
    static_cast<Proiectie&>(*this) = other;
    using std::swap;
    swap(pretBaza_,     other.pretBaza_);
    swap(taxaServicii_, other.taxaServicii_);
    return *this;
}

std::shared_ptr<Proiectie> ProiectieVIP::clone() const {
    return std::make_shared<ProiectieVIP>(*this);
}

std::string ProiectieVIP::descriere_() const {
    return "VIP (open bar + scaune reclinabile)";
}

void ProiectieVIP::afisare_(std::ostream& os) const {
    os << std::fixed << std::setprecision(2)
       << " Pret:" << pretBaza_ << "+"
       << taxaServicii_ << "=" << pretBilet_() << " RON";
}
