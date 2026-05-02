# Sistem Management Cinematograf

Proiect POO in C++ care simuleaza gestiunea unui cinematograf.
Se pot adauga proiectii de tipuri diferite, vinde si anula bilete si vizualiza rapoarte financiare.

Datele se incarca automat din `date.txt` la pornire.

## Ce face programul

La pornire incarca toate salile, proiectiile si biletele din `date.txt`, apoi afiseaza un meniu interactiv cu urmatoarele optiuni:

- **Afisare toate proiectiile** — lista cu tip, data, bilete vandute si incasare
- **Raport detaliat** — detalii complete despre o proiectie, inclusiv eficienta financiara
- **Harta locuri** — vizualizare grafica a locurilor ocupate si libere dintr-o sala
- **Vanzare bilet** — alegi locul si tipul (adult / copil / senior / student), reducerea se calculeaza automat
- **Anulare bilet** — anulezi un bilet dupa ID, locul devine automat liber
- **Chitanta bilet** — afisare chitanta formatata pentru un bilet
- **Cautare dupa titlu** — cauti proiectii dupa un cuvant cheie
- **Top proiectii** — top N proiectii sortate dupa incasare
- **Incasare totala** — suma totala incasata de cinematograf

## Clase

### Bilet

Reprezinta un bilet emis pentru un loc. Retine numarul locului, tipul cumparatorului si pretul de baza. Reducerile se aplica automat: copil -30%, senior -20%, student -15%, adult 0%.

### Sala

Reprezinta o sala de proiectie. Retine capacitatea si starea fiecarui loc (ocupat sau liber). Arunca o exceptie daca incerci sa ocupi un loc deja ocupat sau un index invalid.

### Proiectie (clasa abstracta)

Clasa de baza pentru toate tipurile de proiectii. Nu poate fi creata direct. Contine logica comuna: gestionarea biletelor, calculul incasarilor, afisarea raportului si hartii. Fiecare tip de proiectie derivat defineste cum se calculeaza pretul biletului.

**ProiectieStandard** — proiectie obisnuita 2D sau 3D cu pret fix.

**ProiectieIMAX** — proiectie IMAX cu pret majorat printr-un multiplicator (ex: pret * 1.6).

**ProiectieSpeciala** — eveniment special: premiera, festival sau retrospectiva, cu pret premium fix.

**ProiectieVIP** — sala VIP cu servicii incluse (open bar, scaune reclinabile). Pretul = pret baza + taxa servicii.

### Cinematograf

Clasa principala care gestioneaza toate proiectiile si salile. Retine proiectiile ca pointeri la clasa de baza, ceea ce permite apelarea functiilor virtuale corect pentru fiecare tip. Contine functiile de cautare, sortare si calcul financiar global.

## Exceptii

Programul foloseste o ierarhie proprie de exceptii:

- `EroareBilet` — pret negativ, loc invalid
- `EroareSala` — loc deja ocupat, index invalid, capacitate zero
- `EroareRezervare` — fisier inexistent, titlu gol, index invalid

Toate deriva din `EroareCinematograf`, care deriva din `std::runtime_error`.
