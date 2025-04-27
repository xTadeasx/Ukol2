#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>

class Ucet {
private:
    static const double MAX_VYBER;
    int cisloUctu;
    std::string jmenoMajitele;
    double zustatek;
    int pocetVkladu;
    int pocetVyberu;
    double soucetVkladu;
    double soucetVyberu;
    int maxPocetOperaci;
    std::vector<double> historieVkladu;
    std::vector<double> historieVyberu;

    bool jePlatneCisloUctu(int cislo) const {
        return cislo >= 100001 && cislo <= 109999;
    }

    bool jePlatnaCastka(double castka) const {
        return castka > 0;
    }

public:
    Ucet(int cisloUctu, const std::string& jmenoMajitele, int maxPocetOperaci)
        : cisloUctu(cisloUctu), jmenoMajitele(jmenoMajitele), zustatek(0),
        pocetVkladu(0), pocetVyberu(0), soucetVkladu(0), soucetVyberu(0),
        maxPocetOperaci(maxPocetOperaci) {
        if (!jePlatneCisloUctu(cisloUctu)) {
            throw std::invalid_argument("Neplatne cislo uctu");
        }
    }

    bool vlozit(double castka) {
        if (!jePlatnaCastka(castka)) {
            std::cout << "Neplatna castka pro vklad\n";
            return false;
        }

        if (pocetVkladu + pocetVyberu >= maxPocetOperaci) {
            std::cout << "Dosazen maximalni pocet operaci\n";
            return false;
        }

        zustatek += castka;
        soucetVkladu += castka;
        pocetVkladu++;
        historieVkladu.push_back(castka);
        std::cout << "Vklad " << castka << " Kc byl uspesne proveden\n";
        return true;
    }

    bool vybrat(double castka) {
        if (!jePlatnaCastka(castka)) {
            std::cout << "Neplatna castka pro vyber\n";
            return false;
        }

        if (castka > MAX_VYBER) {
            std::cout << "Vyber překracuje maximalni povolenou castku\n";
            return false;
        }

        if (castka > zustatek) {
            std::cout << "Nedostatek prostredku na uctu\n";
            return false;
        }

        if (pocetVkladu + pocetVyberu >= maxPocetOperaci) {
            std::cout << "Dosazen maximalni pocet operaci\n";
            return false;
        }

        zustatek -= castka;
        soucetVyberu += castka;
        pocetVyberu++;
        historieVyberu.push_back(castka);
        std::cout << "Vyber " << castka << " Kc byl uspesne proveden\n";
        return true;
    }

    void vypisStav() const {
        std::cout << "\n=== STAV UCTU ===\n";
        std::cout << "Cislo uctu: " << cisloUctu << "\n";
        std::cout << "Majitel: " << jmenoMajitele << "\n";
        std::cout << "Zustatek: " << std::fixed << std::setprecision(2) << zustatek << " Kc\n";
        std::cout << "Pocet vkladu: " << pocetVkladu << "\n";
        std::cout << "Pocet vyberu: " << pocetVyberu << "\n";
        std::cout << "Soucet vkladu: " << soucetVkladu << " Kc\n";
        std::cout << "Soucet vyberu: " << soucetVyberu << " Kc\n";
        std::cout << "Maximalni pocet operaci: " << maxPocetOperaci << "\n";
        
        std::cout << "\nHistorie vkladu: ";
        for (double vklad : historieVkladu) {
            std::cout << vklad << "; ";
        }
        std::cout << "\nHistorie vyberu: ";
        for (double vyber : historieVyberu) {
            std::cout << vyber << "; ";
        }
        std::cout << "\n";
    }

    void vypisSouhrn() const {
        std::cout << "\n=== SOUHRN UCTU ===\n";
        std::cout << "Cislo uctu: " << cisloUctu << "\n";
        std::cout << "Majitel: " << jmenoMajitele << "\n";
        std::cout << "Zustatek: " << std::fixed << std::setprecision(2) << zustatek << " Kc\n";
        std::cout << "Pocet vkladu: " << pocetVkladu << "\n";
        std::cout << "Pocet vyberu: " << pocetVyberu << "\n";
        std::cout << "Soucet vkladu: " << soucetVkladu << " Kc\n";
        std::cout << "Soucet vyberu: " << soucetVyberu << " Kc\n";
    }

    void ulozitDoSouboru(const std::string& nazevSouboru) const {
        std::ofstream soubor(nazevSouboru);
        if (!soubor.is_open()) {
            throw std::runtime_error("Nelze otevrit soubor pro zapis");
        }

        soubor << cisloUctu << "\n";
        soubor << jmenoMajitele << "\n";
        soubor << std::fixed << std::setprecision(2) << zustatek << "\n";
        soubor << pocetVkladu << "\n";
        soubor << pocetVyberu << "\n";
        soubor << soucetVkladu << "\n";
        soubor << soucetVyberu << "\n";
        soubor << maxPocetOperaci << "\n";

        for (double vklad : historieVkladu) {
            soubor << vklad << ";";
        }
        soubor << "\n";
        for (double vyber : historieVyberu) {
            soubor << vyber << ";";
        }
        soubor << "\n";
    }

    static Ucet* nactiZeSouboru(const std::string& nazevSouboru) {
        std::ifstream soubor(nazevSouboru);
        if (!soubor.is_open()) {
            throw std::runtime_error("Nelze otevrit soubor pro cteni");
        }

        int cisloUctu, maxPocetOperaci;
        std::string jmenoMajitele;
        double zustatek;
        int pocetVkladu, pocetVyberu;
        double soucetVkladu, soucetVyberu;

        soubor >> cisloUctu;
        soubor.ignore();
        std::getline(soubor, jmenoMajitele);
        soubor >> zustatek >> pocetVkladu >> pocetVyberu >> soucetVkladu >> soucetVyberu >> maxPocetOperaci;

        Ucet* ucet = new Ucet(cisloUctu, jmenoMajitele, maxPocetOperaci);

        return ucet;
    }
};

const double Ucet::MAX_VYBER = 5000.0;

int main(int argc, char* argv[]) {
    Ucet* ucet = nullptr;
    std::string nazevSouboru;

    try {
        if (argc > 1) {
            nazevSouboru = argv[1];
            ucet = Ucet::nactiZeSouboru(nazevSouboru);
        } else {
            int cisloUctu, maxPocetOperaci;
            std::string jmenoMajitele;

            std::cout << "Zadejte cislo uctu (100001-109999): ";
            std::cin >> cisloUctu;
            std::cin.ignore();

            std::cout << "Zadejte jmeno majitele: ";
            std::getline(std::cin, jmenoMajitele);

            std::cout << "Zadejte maximalni pocet operaci: ";
            std::cin >> maxPocetOperaci;

            ucet = new Ucet(cisloUctu, jmenoMajitele, maxPocetOperaci);
        }

        int volba;
        do {
            std::cout << "\n=== MENU ===\n";
            std::cout << "0. Konec programu\n";
            std::cout << "1. Vypis stavu uctu\n";
            std::cout << "2. Vypis souhrnnych informaci\n";
            std::cout << "3. Vlozit castku\n";
            std::cout << "4. Vybrat castku\n";
            std::cout << "Zvolte moznost: ";
            std::cin >> volba;

            switch (volba) {
                case 0:
                    if (nazevSouboru.empty()) {
                        std::cout << "Zadejte nazev souboru pro ulozeni: ";
                        std::cin >> nazevSouboru;
                    }
                    ucet->ulozitDoSouboru(nazevSouboru);
                    break;
                case 1:
                    ucet->vypisStav();
                    break;
                case 2:
                    ucet->vypisSouhrn();
                    break;
                case 3: {
                    double castka;
                    std::cout << "Zadejte castku k vlozeni: ";
                    std::cin >> castka;
                    ucet->vlozit(castka);
                    break;
                }
                case 4: {
                    double castka;
                    std::cout << "Zadejte castku k vyberu: ";
                    std::cin >> castka;
                    ucet->vybrat(castka);
                    break;
                }
                default:
                    std::cout << "Neplatna volba!\n";
            }
        } while (volba != 0);

        delete ucet;
    }
    catch (const std::exception& e) {
        std::cerr << "Chyba: " << e.what() << std::endl;
        delete ucet;
        return 1;
    }

    return 0;
}
