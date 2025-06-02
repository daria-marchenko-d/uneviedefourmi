#include "ants.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <limits> // Nécessaire pour std::numeric_limits
#include "json.hpp"
#include <fstream> 

// Pour utiliser json plus facilement 
using json = nlohmann::json;    


// Fonction pour simuler la Fourmilière 0
void run_colony0() {
    std::cout << "\n--- Fourmilière 0 ---" << std::endl;
    Colony colony0;
    // Utilisez les x, y, width, height de votre visualisation.html pour colony0Data
    // Exemple de valeurs (à ajuster selon votre visualisation.html) :
    colony0.addRoom("Sv", 2,  50, 150, 100, 200); // (name, capacity, x, y, width, height)
    colony0.addRoom("S1",     200, 150, 80, 60);   // (name, x, y, width, height) -> capacité par défaut de Room (1)
    colony0.addRoom("S2",     200, 250, 80, 60);   // (name, x, y, width, height)
    colony0.addRoom("Sd", 2,  350, 150, 100, 200); // (name, capacity, x, y, width, height)

    colony0.setSpecialRooms("Sv", "Sd");
    colony0.addTunnel("Sv", "S1");
    colony0.addTunnel("Sv", "S2");
    colony0.addTunnel("S1", "Sd");
    colony0.addTunnel("S2", "Sd");

    colony0.addAnts(2);
    colony0.computePaths();
    colony0.simulateSteps(100); // Exécute la simulation pour max 100 étapes et construit le JSON interne
    colony0.writeSimulationJSON("simulation_colony0.json"); // Écrit le JSON dans un fichier
}

// Fonction pour simuler la Fourmilière 1
void run_colony1() {
    std::cout << "\n--- Fourmilière 1 ---" << std::endl;
    Colony colony1;
    // Valeurs d'exemple pour x, y, width, height - À AJUSTER
    colony1.addRoom("Sv", 5,  50, 50, 100, 100);
    colony1.addRoom("S1",     160, 50, 80, 60);   // Capacité par défaut
    colony1.addRoom("S2",     250, 50, 80, 60);   // Capacité par défaut
    colony1.addRoom("Sd", 5,  340, 50, 100, 100);
    colony1.setSpecialRooms("Sv", "Sd");
    colony1.addTunnel("Sv", "S1");
    colony1.addTunnel("S1", "S2");
    colony1.addTunnel("S2", "Sd");
    colony1.addAnts(5);
    colony1.computePaths();
    colony1.simulateSteps(100);
    colony1.writeSimulationJSON("simulation_colony1.json");
}

// Fonction pour simuler la Fourmilière 2
void run_colony2() {
    std::cout << "\n--- Fourmilière 2 ---" << std::endl;
    Colony colony2;
    // Valeurs d'exemple pour x, y, width, height - À AJUSTER
    colony2.addRoom("Sv", 5,  50, 200, 100, 100);
    colony2.addRoom("S1",     160, 200, 80, 60);
    colony2.addRoom("S2",     250, 200, 80, 60);
    colony2.addRoom("Sd", 5,  340, 200, 100, 100);
    colony2.setSpecialRooms("Sv", "Sd");
    colony2.addTunnel("Sv", "S1");
    colony2.addTunnel("S1", "S2");
    colony2.addTunnel("S2", "Sd");
    colony2.addTunnel("Sd", "Sv"); // Attention, ce tunnel crée un cycle direct Sd <-> Sv
    colony2.addAnts(5);
    colony2.computePaths();
    colony2.simulateSteps(100);
    colony2.writeSimulationJSON("simulation_colony2.json");
}

// Fonction pour simuler la Fourmilière 3
void run_colony3() {
    std::cout << "\n--- Fourmilière 3 ---" << std::endl;
    Colony colony3;
    // Valeurs d'exemple pour x, y, width, height - À AJUSTER
    colony3.addRoom("Sv", 5,  50, 350, 100, 100);
    colony3.addRoom("S1",     160, 350, 80, 60);
    colony3.addRoom("S2",     250, 350, 80, 60);
    colony3.addRoom("S3",     160, 420, 80, 60);
    colony3.addRoom("S4",     250, 420, 80, 60);
    colony3.addRoom("Sd", 5,  50, 490, 100, 100);
    colony3.setSpecialRooms("Sv", "Sd");
    colony3.addTunnel("Sv", "S1");
    colony3.addTunnel("S1", "S2");
    colony3.addTunnel("S4", "Sd");
    colony3.addTunnel("S1", "S4"); // Peut créer des chemins intéressants
    colony3.addTunnel("S2", "S3"); // S3 n'est pas connecté à Sd directement
    colony3.addAnts(5);
    colony3.computePaths();
    colony3.simulateSteps(100);
    colony3.writeSimulationJSON("simulation_colony3.json");
}

// Fonction pour simuler la Fourmilière 4
void run_colony4() {
    std::cout << "\n--- Fourmilière 4 ---" << std::endl;
    Colony colony4;
    // Valeurs d'exemple pour x, y, width, height - À AJUSTER
    colony4.addRoom("Sv", 10, 50, 550, 100, 100);
    colony4.addRoom("S1", 2,  160, 550, 80, 60);
    colony4.addRoom("S2",     250, 550, 80, 60);
    colony4.addRoom("S3",     160, 620, 80, 60);
    colony4.addRoom("S4", 2,  250, 620, 80, 60);
    colony4.addRoom("S5",     340, 550, 80, 60);
    colony4.addRoom("S6",     340, 620, 80, 60);
    colony4.addRoom("Sd", 10, 430, 585, 100, 100);
    colony4.setSpecialRooms("Sv", "Sd");
    colony4.addTunnel("S3", "S4");
    colony4.addTunnel("Sv", "S1");
    colony4.addTunnel("S1", "S2");
    colony4.addTunnel("S2", "S4"); // Note: S2 se connecte à S4, qui a une capacité de 2
    colony4.addTunnel("S4", "S5");
    colony4.addTunnel("S5", "Sd");
    colony4.addTunnel("S4", "S6");
    colony4.addTunnel("S6", "Sd");
    colony4.addTunnel("S1", "S3");
    colony4.addAnts(10);
    colony4.computePaths();
    colony4.simulateSteps(100);
    colony4.writeSimulationJSON("simulation_colony4.json");
}

// Fonction pour simuler la Fourmilière 5
void run_colony5() {
    std::cout << "\n--- Fourmilière 5 ---" << std::endl;
    Colony colony5;
    // Valeurs d'exemple pour x, y, width, height - À AJUSTER
    // Cette colonie est plus grande, la disposition demandera plus d'attention
    int current_y = 700;
    colony5.addRoom("Sv", 50, 50, current_y, 100, 50);
    colony5.addRoom("S1", 8,  160, current_y, 80, 40);
    colony5.addRoom("S2", 4,  250, current_y, 70, 40);
    colony5.addRoom("S3", 2,  330, current_y, 60, 40);
    colony5.addRoom("S4", 4,  400, current_y, 70, 40);
    current_y += 60;
    colony5.addRoom("S5", 2,  250, current_y, 60, 40); // Sous S2
    colony5.addRoom("S6", 4,  160, current_y + 10, 80, 40); // Sous S1, légèrement décalé
    colony5.addRoom("S7", 2,  250, current_y + 60, 60, 40); // Sous S5
    colony5.addRoom("S8", 5,  50, current_y + 30, 100, 50); // À gauche, plus bas
    current_y += 120;
    colony5.addRoom("S9",     330, current_y, 60, 40); // Capacité par défaut
    colony5.addRoom("S10",    400, current_y, 60, 40); // Capacité par défaut
    colony5.addRoom("S11",    50, current_y, 80, 40);  // Capacité par défaut
    colony5.addRoom("S12",    140, current_y, 80, 40); // Capacité par défaut
    current_y += 50;
    colony5.addRoom("S13", 4, 200, current_y, 100, 50);
    colony5.addRoom("S14", 2, 310, current_y, 100, 50);
    colony5.addRoom("Sd", 50, 500, current_y - 25 , 100, 100); // À droite et un peu plus haut que S13/S14

    colony5.setSpecialRooms("Sv", "Sd");
    colony5.addTunnel("S1", "S2");
    colony5.addTunnel("S2", "S3");
    colony5.addTunnel("S3", "S4");
    colony5.addTunnel("S4", "Sd");
    colony5.addTunnel("Sv", "S1");
    colony5.addTunnel("S2", "S5");
    colony5.addTunnel("S5", "S4"); // Crée un chemin alternatif vers Sd via S4
    colony5.addTunnel("S13", "Sd");
    colony5.addTunnel("S8", "S12");
    colony5.addTunnel("S12", "S13");
    colony5.addTunnel("S6", "S7");
    colony5.addTunnel("S7", "S9");
    colony5.addTunnel("S9", "S14");
    colony5.addTunnel("S14", "Sd");
    colony5.addTunnel("S7", "S10");
    colony5.addTunnel("S10", "S14"); // S10 se connecte aussi à S14
    colony5.addTunnel("S1", "S6");
    colony5.addTunnel("S6", "S8");
    colony5.addTunnel("S8", "S11");
    colony5.addTunnel("S11", "S13");
    colony5.addAnts(50);
    colony5.computePaths();
    colony5.simulateSteps(100);
    colony5.writeSimulationJSON("simulation_colony5.json");
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