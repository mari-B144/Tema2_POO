#ifndef PROIECTIE_H
#define PROIECTIE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Bilet.h"
#include "Sala.h"
#include "Exceptii.h"

// ============================================================
//  Clasa de baza abstracta  Proiectie
//  Ierarhie: Proiectie <-- ProiectieStandard
//                      <-- ProiectieIMAX
//                      <-- ProiectieSpeciala
//                      <-- ProiectieVIP   (adaugata ulterior)
//
//  Interfata non-virtuala (NVI):
//    - pretBilet()      apeleaza pretBilet_()   (virtuala privata pura)
//    - descriere()      apeleaza descriere_()   (virtuala privata pura)
//    - operator<<       apeleaza afisare_()     (virtuala privata)
// ============================================================
class Proiectie {
public:
    virtual ~Proiectie() = default;

    // Constructor virtual (clone)
    virtual std::shared_ptr<Proiectie> clone() const = 0;

    // ── Interfata non-virtuala (publica, non-virtuala) ──────────────────
    // Pret bilet calculat de fiecare tip de proiectie
    double      pretBilet()  const { return pretBilet_(); }
    // Descriere textuala specifica tipului
    std::string descriere()  const { return descriere_(); }
    // Functie specifica temei: calculeaza incasarea estimata (locuri * pret)
    double      incasareEstimata() const;
    // Calculeaza incasarea reala din biletele deja vandute
    double      incasareReala()    const;

    // Afisare prin operator<< → apeleaza afisare_() virtuala
    friend std::ostream& operator<<(std::ostream& os, const Proiectie& p);

    // ── Operatii pe bilete ──────────────────────────────────────────────
    // arunca EroareSala sau EroareBilet
    void   adaugaBilet(int nrLoc, Bilet::Tip tip);
    bool   anuleazaBilet(int idBilet);
    void   afisareChitanta(int idBilet) const;
    bool   areLocuriDisponibile() const { return sala_.locuriDisponibile() > 0; }

    // ── Getteri ─────────────────────────────────────────────────────────
    const std::string& getTitluFilm() const { return titluFilm_; }
    const std::string& getDataOra()   const { return dataOra_; }
    const Sala&        getSala()       const { return sala_; }
    int                getNrBilete()   const { return static_cast<int>(bilete_.size()); }
    int                getId()         const { return id_; }

    void   afisareRaport() const;
    void   afisareHartaSala() const { sala_.afiseazaHarta(); }

    static int totalProiectii() { return contor_; }

public:
    Proiectie& operator=(const Proiectie& other);
protected:
    // Constructor apelat din derivate
    Proiectie(const std::string& titluFilm,
              const Sala& sala,
              const std::string& dataOra);

    // cc/op= protected — previne object slicing, permite copierea corecta in derivate
    Proiectie(const Proiectie& other);
    friend void swap(Proiectie& a, Proiectie& b) noexcept;

private:
    // ── Functii virtuale private (NVI) ──────────────────────────────────
    virtual double      pretBilet_()  const = 0;
    virtual std::string descriere_()  const = 0;
    virtual void        afisare_(std::ostream& os) const;   // nu e pura, are impl default

    // ── Date membre ─────────────────────────────────────────────────────
    int                        id_;
    std::string                titluFilm_;
    Sala                       sala_;
    std::string                dataOra_;
    std::vector<Bilet>         bilete_;

    static int contor_;
    static int urmatorulId_;
};

// ============================================================
//  ProiectieStandard  (2D sau 3D, pret de baza)
// ============================================================
class ProiectieStandard : public Proiectie {
public:
    ProiectieStandard(const std::string& titlu,
                      const Sala& sala,
                      const std::string& dataOra,
                      double pretBaza);
    ProiectieStandard(const ProiectieStandard& other);
    ProiectieStandard& operator=(ProiectieStandard other);

    std::shared_ptr<Proiectie> clone() const override;

private:
    double pretBaza_;

    double      pretBilet_()  const override { return pretBaza_; }
    std::string descriere_()  const override;
    void        afisare_(std::ostream& os) const override;
};

// ============================================================
//  ProiectieIMAX  (pret majorat cu multiplicator)
// ============================================================
class ProiectieIMAX : public Proiectie {
public:
    ProiectieIMAX(const std::string& titlu,
                  const Sala& sala,
                  const std::string& dataOra,
                  double pretBaza,
                  double multiplicator = 1.5);
    ProiectieIMAX(const ProiectieIMAX& other);
    ProiectieIMAX& operator=(ProiectieIMAX other);

    std::shared_ptr<Proiectie> clone() const override;

private:
    double pretBaza_;
    double multiplicator_;

    double      pretBilet_()  const override { return pretBaza_ * multiplicator_; }
    std::string descriere_()  const override;
    void        afisare_(std::ostream& os) const override;
};

// ============================================================
//  ProiectieSpeciala  (premiere, festival — locuri VIP, pret premium)
// ============================================================
class ProiectieSpeciala : public Proiectie {
public:
    enum class TipEveniment { Premiera, Festival, Retrospectiva };

    ProiectieSpeciala(const std::string& titlu,
                      const Sala& sala,
                      const std::string& dataOra,
                      double pretPremium,
                      TipEveniment eveniment);
    ProiectieSpeciala(const ProiectieSpeciala& other);
    ProiectieSpeciala& operator=(ProiectieSpeciala other);

    std::shared_ptr<Proiectie> clone() const override;

    static std::string evenimentToString(TipEveniment t);

private:
    double       pretPremium_;
    TipEveniment eveniment_;

    double      pretBilet_()  const override { return pretPremium_; }
    std::string descriere_()  const override;
    void        afisare_(std::ostream& os) const override;
};

#endif // PROIECTIE_H
