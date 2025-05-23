#include "ants.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <limits> // Nécessaire pour std::numeric_limits

// Fonction pour simuler la Fourmilière 0
void run_colony0() {
    std::cout << "\n--- Fourmilière 0 ---" << std::endl;
    Colony colony0;
    colony0.addRoom("Sv", 2);
    colony0.addRoom("S1");
    colony0.addRoom("S2");
    colony0.addRoom("Sd", 2);
    colony0.setSpecialRooms("Sv", "Sd");
    colony0.addTunnel("Sv", "S1");
    colony0.addTunnel("Sv", "S2");
    colony0.addTunnel("S1", "Sd");
    colony0.addTunnel("S2", "Sd");
    colony0.addAnts(2);
    colony0.computePaths();
    colony0.simulateSteps();
}

// Fonction pour simuler la Fourmilière 1
void run_colony1() {
    std::cout << "\n--- Fourmilière 1 ---" << std::endl;
    Colony colony1;
    colony1.addRoom("Sv", 5);
    colony1.addRoom("S1");
    colony1.addRoom("S2");
    colony1.addRoom("Sd", 5);
    colony1.setSpecialRooms("Sv", "Sd");
    colony1.addTunnel("Sv", "S1");
    colony1.addTunnel("S1", "S2");
    colony1.addTunnel("S2", "Sd");
    colony1.addAnts(5);
    colony1.computePaths();
    colony1.simulateSteps();
}

// Fonction pour simuler la Fourmilière 2
void run_colony2() {
    std::cout << "\n--- Fourmilière 2 ---" << std::endl;
    Colony colony2;
    colony2.addRoom("Sv", 5);
    colony2.addRoom("S1");
    colony2.addRoom("S2");
    colony2.addRoom("Sd", 5);
    colony2.setSpecialRooms("Sv", "Sd");
    colony2.addTunnel("Sv", "S1");
    colony2.addTunnel("S1", "S2");
    colony2.addTunnel("S2", "Sd");
    colony2.addTunnel("Sd", "Sv");
    colony2.addAnts(5);
    colony2.computePaths();
    colony2.simulateSteps();
}

// Fonction pour simuler la Fourmilière 3
void run_colony3() {
    std::cout << "\n--- Fourmilière 3 ---" << std::endl;
    Colony colony3;
    colony3.addRoom("Sv", 5);
    colony3.addRoom("S1");
    colony3.addRoom("S2");
    colony3.addRoom("S3");
    colony3.addRoom("S4");
    colony3.addRoom("Sd", 5);
    colony3.setSpecialRooms("Sv", "Sd");
    colony3.addTunnel("Sv", "S1");
    colony3.addTunnel("S1", "S2");
    colony3.addTunnel("S4", "Sd");
    colony3.addTunnel("S1", "S4");
    colony3.addTunnel("S2", "S3");
    colony3.addAnts(5);
    colony3.computePaths();
    colony3.simulateSteps();
}

// Fonction pour simuler la Fourmilière 4
void run_colony4() {
    std::cout << "\n--- Fourmilière 4 ---" << std::endl;
    Colony colony4;
    colony4.addRoom("Sv", 10);
    colony4.addRoom("S1", 2);
    colony4.addRoom("S2");
    colony4.addRoom("S3");
    colony4.addRoom("S4", 2);
    colony4.addRoom("S5");
    colony4.addRoom("S6");
    colony4.addRoom("Sd", 10);
    colony4.setSpecialRooms("Sv", "Sd");
    colony4.addTunnel("S3", "S4");
    colony4.addTunnel("Sv", "S1");
    colony4.addTunnel("S1", "S2");
    colony4.addTunnel("S2", "S4");
    colony4.addTunnel("S4", "S5");
    colony4.addTunnel("S5", "Sd");
    colony4.addTunnel("S4", "S6");
    colony4.addTunnel("S6", "Sd");
    colony4.addTunnel("S1", "S3");
    colony4.addAnts(10);
    colony4.computePaths();
    colony4.simulateSteps();
}

// Fonction pour simuler la Fourmilière 5
void run_colony5() {
    std::cout << "\n--- Fourmilière 5 ---" << std::endl;
    Colony colony5;
    colony5.addRoom("Sv", 50);
    colony5.addRoom("S1", 8);
    colony5.addRoom("S2", 4);
    colony5.addRoom("S3", 2);
    colony5.addRoom("S4", 4);
    colony5.addRoom("S5", 2);
    colony5.addRoom("S6", 4);
    colony5.addRoom("S7", 2);
    colony5.addRoom("S8", 5);
    colony5.addRoom("S9");
    colony5.addRoom("S10");
    colony5.addRoom("S11");
    colony5.addRoom("S12");
    colony5.addRoom("S13", 4);
    colony5.addRoom("S14", 2);
    colony5.addRoom("Sd", 50);
    colony5.setSpecialRooms("Sv", "Sd");
    colony5.addTunnel("S1", "S2");
    colony5.addTunnel("S2", "S3");
    colony5.addTunnel("S3", "S4");
    colony5.addTunnel("S4", "Sd");
    colony5.addTunnel("Sv", "S1");
    colony5.addTunnel("S2", "S5");
    colony5.addTunnel("S5", "S4");
    colony5.addTunnel("S13", "Sd");
    colony5.addTunnel("S8", "S12");
    colony5.addTunnel("S12", "S13");
    colony5.addTunnel("S6", "S7");
    colony5.addTunnel("S7", "S9");
    colony5.addTunnel("S9", "S14");
    colony5.addTunnel("S14", "Sd");
    colony5.addTunnel("S7", "S10");
    colony5.addTunnel("S10", "S14");
    colony5.addTunnel("S1", "S6");
    colony5.addTunnel("S6", "S8");
    colony5.addTunnel("S8", "S11");
    colony5.addTunnel("S11", "S13");
    colony5.addAnts(50);
    colony5.computePaths();
    colony5.simulateSteps();
}

int main() {
    int choice;
    std::cout << "Simulation de fourmilières" << std::endl;

    while (true) {
        std::cout << "\nChoisissez une fourmilière à simuler :" << std::endl;
        std::cout << "  0: Fourmilière 0" << std::endl;
        std::cout << "  1: Fourmilière 1" << std::endl;
        std::cout << "  2: Fourmilière 2" << std::endl;
        std::cout << "  3: Fourmilière 3" << std::endl;
        std::cout << "  4: Fourmilière 4" << std::endl;
        std::cout << "  5: Fourmilière 5" << std::endl;
        std::cout << "  Entrez tout autre nombre pour quitter." << std::endl;
        std::cout << "Votre choix : ";

        if (!(std::cin >> choice)) {
            std::cout << "Entrée invalide. Veuillez entrer un nombre." << std::endl;
            std::cin.clear(); // Efface les indicateurs d'erreur
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore le reste de la ligne incorrecte
            continue; // Redemande une entrée
        }

        switch (choice) {
            case 0:
                run_colony0();
                break;
            case 1:
                run_colony1();
                break;
            case 2:
                run_colony2();
                break;
            case 3:
                run_colony3();
                break;
            case 4:
                run_colony4();
                break;
            case 5:
                run_colony5();
                break;
            default:
                std::cout << "Fin de la simulation. Au revoir !" << std::endl;
                return 0; // Quitte le programme
        }
        std::cout << "\n----------------------------------------\n"; // Séparateur après une simulation
    }

    return 0; // Ne devrait pas être atteint si la boucle est infinie et quitte via le cas par défaut
}