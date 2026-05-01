#include "Sala.h"
#include <iomanip>
#include <algorithm>
#include <numeric>

int Sala::contor_ = 0;

Sala::Sala()
    : id_(0), nume_("Default"), locuri_(50, false), tip_(Tip::Standard2D)
{ ++contor_; }

Sala::Sala(int id, const std::string& nume, int capacitate, Tip tip)
    : id_(id), nume_(nume), locuri_(capacitate, false), tip_(tip)
{
    if (capacitate <= 0)
        throw EroareSala("capacitatea salii trebuie sa fie pozitiva, primita: "
                         + std::to_string(capacitate));
    ++contor_;
}

Sala& Sala::operator=(Sala other) {
    swap(*this, other);
    return *this;
}

void swap(Sala& a, Sala& b) noexcept {
    using std::swap;
    swap(a.id_,     b.id_);
    swap(a.nume_,   b.nume_);
    swap(a.locuri_, b.locuri_);
    swap(a.tip_,    b.tip_);
}

std::ostream& operator<<(std::ostream& os, const Sala& s) {
    os << "[Sala] ID:" << s.id_
       << " \"" << s.nume_ << "\""
       << " Tip:" << Sala::tipToString(s.tip_)
       << " Cap:" << s.getCapacitate()
       << " Libere:" << s.locuriDisponibile()
       << " Ocupare:" << std::fixed << std::setprecision(1)
       << s.procentOcupare() << "%";
    return os;
}

void Sala::verificaIndex(int idx) const {
    if (idx < 0 || idx >= static_cast<int>(locuri_.size()))
        throw EroareSala("index loc invalid: " + std::to_string(idx + 1)
                         + " (sala are " + std::to_string(locuri_.size()) + " locuri)");
}

void Sala::ocupaLoc(int idx) {
    verificaIndex(idx);
    if (locuri_[idx])
        throw EroareSala("locul " + std::to_string(idx + 1)
                         + " din sala \"" + nume_ + "\" este deja ocupat");
    locuri_[idx] = true;
}

void Sala::elibereazaLoc(int idx) {
    verificaIndex(idx);
    locuri_[idx] = false;
}

bool Sala::esteDisponibil(int idx) const {
    verificaIndex(idx);
    return !locuri_[idx];
}

int Sala::locuriDisponibile() const {
    return static_cast<int>(std::count(locuri_.begin(), locuri_.end(), false));
}

float Sala::procentOcupare() const {
    if (locuri_.empty()) return 0.0f;
    int ocupate = static_cast<int>(std::count(locuri_.begin(), locuri_.end(), true));
    return static_cast<float>(ocupate) / locuri_.size() * 100.0f;
}

void Sala::afiseazaHarta() const {
    const int PE_RAND = 10;
    std::cout << "\n  Harta \"" << nume_ << "\" [O=ocupat  L=liber]\n  ";
    for (int i = 0; i < static_cast<int>(locuri_.size()); ++i) {
        std::cout << (locuri_[i] ? " O" : " L");
        if ((i + 1) % PE_RAND == 0) std::cout << "\n  ";
    }
    std::cout << "\n";
}

void Sala::resetLocuri() {
    std::fill(locuri_.begin(), locuri_.end(), false);
}

std::string Sala::tipToString(Tip t) {
    switch (t) {
        case Tip::Standard2D: return "2D";
        case Tip::Standard3D: return "3D";
        case Tip::IMAX:       return "IMAX";
        default:              return "necunoscut";
    }
}
