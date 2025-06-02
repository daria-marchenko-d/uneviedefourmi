# uneviedefourmi

En français:

SIMULATION DE FOURMILIÈRES EN C++

PROBLÉMATIQUE

Le but de ce projet est de simuler le déplacement de plusieurs fourmis à travers une fourmilière, modélisée comme un graphe de salles reliées par des tunnels. Chaque salle a une capacité maximale, et les fourmis doivent partir d’un vestibule (Sv) pour atteindre un dortoir (Sd), en évitant les blocages ou la congestion.

Le défi principal consiste à déterminer une stratégie de déplacement optimale et synchronisée pour les fourmis, dans des configurations de fourmilières variées (du simple au complexe), en respectant les contraintes de capacité des salles.

SOLUTION APPORTÉE

Le programme repose sur trois fichiers principaux :
- main.cpp : contient les définitions des différentes colonies (colony0 à colony5) et leur exécution.
- ants.cpp : implémente la logique des fourmis, des salles et des colonies (ajout de tunnels, déplacement, etc.).
- ants.hpp : définitions des classes Room, Ant et Colony.

Chaque colonie est construite dynamiquement avec ses propres salles, tunnels et nombre de fourmis.

La simulation se fait étape par étape : à chaque étape, les fourmis qui peuvent avancer le font, en respectant la capacité de la salle cible. Une fois toutes les fourmis arrivées au dortoir, la simulation s’arrête. Un affichage console retrace chaque déplacement et l’état des salles.

ALGORITHME UTILISÉ

Le déplacement est basé sur un parcours en largeur (BFS) :
- Lors de l’initialisation (computePaths()), chaque fourmi calcule le plus court chemin du vestibule au dortoir.
- À chaque étape :
  - On identifie les fourmis qui peuvent se déplacer vers leur prochaine salle.
  - Si la salle cible n’est pas pleine (ou est le dortoir), et qu’elle n’est pas déjà ciblée par une autre fourmi, le déplacement est autorisé.
  - Une fois le déplacement effectué, on met à jour les salles et l’affichage.
- Si aucune fourmi ne peut bouger pendant plus de 100 étapes, la simulation s’interrompt pour éviter les boucles infinies.

ANALYSE DES COLONIES SIMULÉES

Fourmilière 0
Structure : Sv → S1/S2 → Sd
Nombre de fourmis : 2
Capacité des salles : 2 pour Sv et Sd, 1 pour S1 et S2
Caractéristiques :
Cette colonie forme un "Y". Chaque fourmi prend l’un des deux chemins disponibles. Grâce au BFS, elles se répartissent efficacement.

Fourmilière 1
Structure : Sv → S1 → S2 → Sd
Nombre de fourmis : 5
Caractéristiques :
Un couloir étroit sans branches. La capacité limitée de S1 et S2 crée un goulot d’étranglement. Seule une fourmi peut avancer à la fois, ce qui ralentit la simulation.

Fourmilière 2
Structure : Sv → S1 → S2 → Sd → Sv (cycle)
Nombre de fourmis : 5
Caractéristiques :
Même structure que la 1, mais avec un cycle fermé. Le BFS empêche les fourmis de tourner en boucle, en choisissant le chemin le plus court.

Fourmilière 3
Structure :
  - Sv → S1 → S2 → S3 (impasse)
  - Sv → S1 → S4 → Sd
Nombre de fourmis : 5
Caractéristiques :
Un faux chemin mène à une impasse. Le BFS guide les fourmis uniquement par le bon chemin. Utile pour tester la robustesse du calcul de chemins.

Fourmilière 4
Structure : Plusieurs chemins :
  - Sv → S1 → S2 → S4 → S5 → Sd
  - Sv → S1 → S3 → S4 → S6 → Sd
Nombre de fourmis : 10
Caractéristiques :
Architecture complexe avec des bifurcations. Les fourmis s’adaptent à la topologie pour éviter la congestion.

Fourmilière 5
Structure : Très dense : 14 salles + vestibule + dortoir
Nombre de fourmis : 50
Caractéristiques :
Plusieurs chemins interconnectés, des capacités différentes par salle. Test grandeur nature du système : congestion, synchronisation, efficacité.

CONCLUSION

Ce projet simule avec succès des scénarios variés de déplacement de fourmis à travers des fourmilières de complexité croissante. Il démontre :
- L’efficacité du BFS dans des graphes dirigés et pondérés par des capacités.
- L’importance de la coordination et de la gestion des conflits de ressources.
- La flexibilité du système à s’adapter à différentes topologies.

Cette simulation peut servir de base à d’autres applications multi-agents ou en logistique, où des entités doivent parcourir un réseau de manière optimale.

COMPILATION

g++ -std=c++17 -o fourmis_simulation main.cpp ants.cpp
./fourmis_simulation

In English:


ANT HILL SIMULATION IN C++

PROBLEM

The goal of this project is to simulate the movement of several ants through an anthill, modeled as a graph of rooms connected by tunnels. Each room has a maximum capacity, and the ants must start from a vestibule (Sv) to reach a dormitory (Sd), avoiding blockages or congestion.

The main challenge is to determine an optimal and synchronized movement strategy for the ants, in various anthill configurations (from simple to complex), while respecting the room capacity constraints.

SOLUTION PROVIDED

The program is based on three main files:
- main.cpp: contains the definitions of the different colonies (colony0 to colony5) and their execution.
- ants.cpp: implements the logic of the ants, rooms, and colonies (adding tunnels, moving, etc.). - ants.hpp: definitions of the Room, Ant, and Colony classes.

Each colony is dynamically built with its own rooms, tunnels, and number of ants.

The simulation is performed step by step: at each step, the ants that can move forward do so, respecting the capacity of the target room. Once all the ants have reached the dormitory, the simulation stops. A console display tracks each movement and the status of the rooms.

ALGORITHM USED

Movement is based on a breadth-first path (BFS):
- During initialization (computePaths()), each ant calculates the shortest path from the vestibule to the dormitory.
- At each step:
- We identify the ants that can move to their next room.
- If the target room is not full (or is the dormitory), and it is not already targeted by another ant, movement is allowed. - Once the move is complete, the rooms and the display are updated.
- If no ant can move for more than 100 steps, the simulation stops to avoid infinite loops.

ANALYSIS OF SIMULATED COLONY

Anthill 0
Structure: Sv → S1/S2 → Sd
Number of ants: 2
Room capacity: 2 for Sv and Sd, 1 for S1 and S2
Characteristics:
This colony forms a "Y." Each ant takes one of the two available paths. Thanks to the BFS, they are distributed efficiently.

Anthill 1
Structure: Sv → S1 → S2 → Sd
Number of ants: 5
Characteristics:
A narrow corridor with no branches. The limited capacity of S1 and S2 creates a bottleneck. Only one ant can move at a time, which slows down the simulation.

Anthill 2
Structure: Sv → S1 → S2 → Sd → Sv (cycle)
Number of ants: 5
Features:
Same structure as 1, but with a closed cycle. The BFS prevents the ants from looping by choosing the shortest path.

Anthill 3
Structure:
- Sv → S1 → S2 → S3 (dead end)
- Sv → S1 → S4 → Sd
Number of ants: 5
Features:
A false path leads to a dead end. The BFS guides the ants only along the correct path. Useful for testing the robustness of path computation.

Anthill 4
Structure: Multiple paths:
- Sv → S1 → S2 → S4 → S5 → Sd
- Sv → S1 → S3 → S4 → S6 → Sd
Number of ants: 10
Features:
Complex architecture with branching paths. Ants adapt to the topology to avoid congestion.

Anthill 5
Structure: Very dense: 14 rooms + vestibule + dormitory
Number of ants: 50
Features:
Multiple interconnected paths, different capacities per room. Full-scale system testing: congestion, synchronization, efficiency.

CONCLUSION

This project successfully simulates various scenarios of ant movement through anthills of increasing complexity. It demonstrates:
- The effectiveness of BFS in directed and capacity-weighted graphs.
- The importance of coordination and resource conflict management.
- The system's flexibility in adapting to different topologies.

This simulation can serve as a basis for other multi-agent or logistics applications, where entities must navigate a network optimally.

COMPILATION

g++ -std=c++17 -o fourmis_simulation main.cpp ants.cpp
./fourmis_simulation
