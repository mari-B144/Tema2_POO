#ifndef PROIECTIEVIP_H
#define PROIECTIEVIP_H

#include "Proiectie.h"

// ============================================================
//  ProiectieVIP  -  a 4-a clasa derivata, adaugata separat
//  Sala VIP cu servicii premium (open bar, scaune reclinabile)
//  Pret = pretBaza + taxaServicii
// ============================================================
class ProiectieVIP : public Proiectie {
public:
    ProiectieVIP(const std::string& titlu,
                 const Sala& sala,
                 const std::string& dataOra,
                 double pretBaza,
                 double taxaServicii = 30.0);

    ProiectieVIP(const ProiectieVIP& other);
    ProiectieVIP& operator=(ProiectieVIP other);

    std::shared_ptr<Proiectie> clone() const override;

private:
    double pretBaza_;
    double taxaServicii_;

    double      pretBilet_()  const override { return pretBaza_ + taxaServicii_; }
    std::string descriere_()  const override;
    void        afisare_(std::ostream& os) const override;
};

#endif // PROIECTIEVIP_H
