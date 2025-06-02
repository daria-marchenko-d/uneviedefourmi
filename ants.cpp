#include "ants.hpp"
#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <set>
#include "json.hpp"
#include <fstream> // Pour écrire dans un fichier

// Pour utiliser json plus facilement
using json = nlohmann::json;

// ---------- ROOM METHODS ----------

Room::Room(const std::string &name, int capacity, int vis_x, int vis_y, int vis_w, int vis_h)
    : name(name), capacity(capacity), x(vis_x), y(vis_y), width(vis_w), height(vis_h) {}

bool Room::isFull() const
{
    return antsInside.size() >= static_cast<size_t>(capacity);
}


void Room::addAnt(Ant *ant)
{
    if (!isFull()) {
        antsInside.push_back(ant);
    }
}

void Room::removeAnt(Ant *ant)
{
    for (size_t i = 0; i < antsInside.size(); ++i) {
        if (antsInside[i] == ant) {
            antsInside.erase(antsInside.begin() + i);
            break;
        }
    }
}

bool Room::hasNeighbor(Room *room) const
{
    for (const auto& neighbor_ptr : neighbors) {
        if (neighbor_ptr == room) {
            return true;
        }
    }
    return false;
}

// ---------- ANT METHODS ----------

Ant::Ant(const std::string &id, Room *startRoom)
    : id(id), currentRoom(startRoom), pathIndex(0) {}

bool Ant::hasArrived(Room *dormitory) const
{
    return currentRoom == dormitory;
}

Room *Ant::getNextRoom() const
{
    if (pathIndex < static_cast<int>(path.size()))
    {
        return path[pathIndex];
    }
    return nullptr;
}

void Ant::advance()
{
    if (pathIndex < static_cast<int>(path.size()))
    {
        currentRoom = path[pathIndex];
        pathIndex++;
    }
}

// ---------- COLONY METHODS ----------

void Colony::addRoom(const std::string& name, int capacity, int x, int y, int width, int height) {
    // Vérifie si la salle existe déjà (pour std::map)
    if (rooms.count(name)) {
        std::cerr << "Erreur : La salle " << name << " existe déjà." << std::endl;
        return;
    }
    // Le constructeur Room prend (name, capacity, vis_x, vis_y, vis_w, vis_h)
    rooms.emplace(name, Room(name, capacity, x, y, width, height));
}

// NOUVELLE SURCHARGE de addRoom avec capacité par défaut et propriétés visuelles
void Colony::addRoom(const std::string& name, int x, int y, int width, int height) {
    // Vérifie si la salle existe déjà (pour std::map)
    if (rooms.count(name)) {
        std::cerr << "Erreur : La salle " << name << " existe déjà." << std::endl;
        return;
    }
    // Le constructeur Room a une capacité par défaut de 1
    // Room(const std::string &name, int capacity = 1, int vis_x = 0, int vis_y = 0, int vis_w = 0, int vis_h = 0);
    // Donc on peut appeler avec la capacité par défaut (1)
    rooms.emplace(name, Room(name, 1, x, y, width, height));
}

void Colony::addTunnel(const std::string &from, const std::string &to)
{
    Room *roomFrom = &rooms[from];
    Room *roomTo = &rooms[to];
    roomFrom->neighbors.push_back(roomTo);
    roomTo->neighbors.push_back(roomFrom); // bidirectional
}

void Colony::setSpecialRooms(const std::string &vst, const std::string &dor)
{
    vestibule = &rooms[vst];
    dormitory = &rooms[dor];
}

void Colony::addAnts(int count)
{
    for (int i = 1; i <= count; ++i)
    {
        std::string id = "f" + std::to_string(i);
        ants.emplace_back(id, vestibule);
        vestibule->addAnt(&ants.back());
    }
}

// Basic BFS to compute shortest path from start to dormitory
std::vector<Room *> bfsPath(Room *start, Room *goal)
{
    std::unordered_map<Room *, Room *> prev;
    std::unordered_set<Room *> visited;
    std::queue<Room *> q;

    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        Room *current = q.front();
        q.pop();

        if (current == goal)
            break;

        for (Room *neighbor : current->neighbors)
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                prev[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    std::vector<Room *> path;
    Room *cur = goal;

    while (prev.find(cur) != prev.end())
    {
        path.push_back(cur);
        cur = prev[cur];
    }

    if (cur == start)
    {
        std::reverse(path.begin(), path.end());
        return path;
    }

    return {}; // No path found
}

void Colony::computePaths()
{
    for (Ant &ant : ants)
    {
        ant.path = bfsPath(ant.currentRoom, dormitory);
        ant.pathIndex = 0;
    }
}

void Colony::simulateSteps(int max_steps)
{

    simulation_output_json_ = json::object(); // S'assurer qu'il est vide et est un objet

    json rooms_json_array = json::array(); // Renommé pour clarté
    // Pour un ordre constant des salles dans le JSON, on peut trier leurs noms
    std::vector<std::string> room_keys_sorted;
    for(const auto& pair : this->rooms) { // 'this->rooms' est votre std::unordered_map<std::string, Room>
        room_keys_sorted.push_back(pair.first);
    }
    std::sort(room_keys_sorted.begin(), room_keys_sorted.end());

    for (const auto& room_key : room_keys_sorted) {
        const Room& room_obj = this->rooms.at(room_key); // 'room_obj' pour éviter conflit avec 'room' dans la boucle interne
        rooms_json_array.push_back({
            {"name", room_obj.name},
            {"x", room_obj.x},
            {"y", room_obj.y},
            {"width", room_obj.width},
            {"height", room_obj.height},
            {"capacity", room_obj.capacity}
        });
    }
    simulation_output_json_["rooms"] = rooms_json_array;

    json tunnels_json_array = json::array(); // Renommé pour clarté
    std::set<std::pair<std::string, std::string>> unique_tunnels_set; // Renommé pour clarté
    for (const auto& room_key : room_keys_sorted) {
        const Room& room_obj = this->rooms.at(room_key);
        for (const Room* neighbor_ptr : room_obj.neighbors) { // 'neighbor_ptr' pour clarté
            std::string s1 = room_obj.name;
            std::string s2 = neighbor_ptr->name;
            if (s1 > s2) std::swap(s1, s2);
            if (unique_tunnels_set.find({s1, s2}) == unique_tunnels_set.end()) {
                 tunnels_json_array.push_back({{"from", room_obj.name}, {"to", neighbor_ptr->name}});
                 unique_tunnels_set.insert({s1,s2});
            }
        }
    }
    simulation_output_json_["tunnels"] = tunnels_json_array;

    json simulation_steps_array_json = json::array(); // Renommé pour clarté

    std::cout << "Début de la simulation pour JSON..." << std::endl;

    // Étape 0 (état initial)
    json initial_ants_state_json = json::array(); // Renommé pour clarté
    for (const auto& ant_obj : this->ants) { // 'ant_obj' pour éviter conflit
        initial_ants_state_json.push_back({
            {"id", ant_obj.id},
            {"currentRoomName", ant_obj.currentRoom ? ant_obj.currentRoom->name : "null"}
        });
    }
    simulation_steps_array_json.push_back({{"step", 0}, {"ants", initial_ants_state_json}});
    std::cout << "Étape 0: Positions initiales des fourmis enregistrées pour JSON." << std::endl;

    int step = 1;
    while (!allArrived())
    {
        std::vector<std::tuple<std::string, std::string, std::string>> current_step_movements;
        std::vector<Ant*>ants_to_move_this_step; // Pour stocker les fourmis qui vont bouger
        std::vector<Room*> next_rooms_for_ants_to_move; // Etleur destinations

        // Phase 1: Identifier les fourmis qui peuvent et vont bouger
        for (size_t i = 0; i < this->ants.size(); ++i)
        {
            Ant* ant_ptr = &this->ants[i];
            if (!ant_ptr || ant_ptr->hasArrived(this->dormitory) || !ant_ptr->currentRoom) // this->dormitory
            {
                continue;
            }

            Room* next_room_candidate = ant_ptr->getNextRoom();
            if (next_room_candidate)
            {
                if (!next_room_candidate->isFull() || next_room_candidate == this->dormitory)
                {
                    bool room_already_targeted = false;
                    if (next_room_candidate != this->dormitory) {
                        for(const auto& planned_move_room : next_rooms_for_ants_to_move) {
                            if (planned_move_room == next_room_candidate) {
                                room_already_targeted = true;
                                break;
                            }
                        }
                    }

                    if (!room_already_targeted) {
                        ants_to_move_this_step.push_back(ant_ptr);
                        next_rooms_for_ants_to_move.push_back(next_room_candidate);
                    }
                }
            }
        }

        // Phase 2: Effectuer les mouvements identifiés
        if (!ants_to_move_this_step.empty()) {
            for (size_t i = 0; i < ants_to_move_this_step.size(); ++i) {
                Ant* ant_ptr = ants_to_move_this_step[i];
                Room* next_room = next_rooms_for_ants_to_move[i];
                Room* old_room = ant_ptr->currentRoom;

                if (old_room) {
                    old_room->removeAnt(ant_ptr);
                }
                next_room->addAnt(ant_ptr);
                ant_ptr->advance();

                current_step_movements.emplace_back(ant_ptr->id, old_room->name, ant_ptr->currentRoom->name);                 
            }
        }

        json current_ants_state_for_json_step = json::array(); // Renommé pour clarté
        for (const auto& ant_obj : this->ants) { // 'ant_obj' pour éviter conflit
            current_ants_state_for_json_step.push_back({
                {"id", ant_obj.id},
                {"currentRoomName", ant_obj.currentRoom ? ant_obj.currentRoom->name : "null"}
            });
        }
        simulation_steps_array_json.push_back({{"step", step}, {"ants", current_ants_state_for_json_step}});

        printStep(step, current_step_movements);
        
        if (ants_to_move_this_step.empty() && !allArrived()) {
            std::cout << "           (Aucune fourmi n'a bougé à l'étape " << step << " mais toutes ne sont pas arrivées)" << std::endl;
        }

        step++;
        if (step > max_steps && !allArrived()) { // Utiliser max_steps du paramètre
            std::cout << "Simulation arrêtée après " << max_steps << " étapes (possible blocage)." << std::endl;
            break;
        }
    }

    simulation_output_json_["simulation_steps"] = simulation_steps_array_json;

    json final_ants_state_json = json::array();
    for (const auto& ant_obj : this->ants) { // Utilise l'état actuel de this->ants, qui devrait être l'état final
        final_ants_state_json.push_back({
            {"id", ant_obj.id},
            {"currentRoomName", ant_obj.currentRoom ? ant_obj.currentRoom->name : "null"}
        });
    }
    simulation_output_json_["ants"] = final_ants_state_json;


    std::cout << "\nFin de la simulation pour JSON." << std::endl;
}

void Colony::printStep(int stepNumber, const std::vector<std::tuple<std::string, std::string, std::string>>& movements) const
{
    
    if (!movements.empty()) // On affiche l'entête et les mouvements seulement s'il y en a
    {
        std::cout << "           +++ E " << stepNumber << " +++" << std::endl;
        for (const auto& move : movements)
        {
            std::cout << "           " << std::get<0>(move) << " - " << std::get<1>(move) << " - " << std::get<2>(move) << std::endl;
        }
    } else if (stepNumber == 1 && movements.empty()) { // Cas spécial: première étape sans mouvement
         std::cout << "           +++ E " << stepNumber << " +++" << std::endl;
         std::cout << "           (Aucune fourmi n'a bougé)" << std::endl;
    }
    // Si ce n'est pas la première étape et qu'il n'y a pas de mouvements, on n'affiche rien pour les mouvements.

    // NOUVELLE PARTIE : Afficher l'état de toutes les salles
    // On affiche l'état des salles seulement si des mouvements ont eu lieu, ou si c'est la première étape.
    // Ou, on peut décider de toujours afficher l'état des salles.
    // Pour l'instant, lions cela à la condition qu'il y ait eu des mouvements ou que ce soit la première étape.
    if (!movements.empty() || stepNumber == 1) {
        std::cout << "           --- Etat des salles ---" << std::endl;
        // Pour avoir un ordre d'affichage constant, il serait mieux de trier les noms des salles.
        // Créons une version triée des noms de salles pour l'affichage.
        std::vector<std::string> room_names;
        for (const auto& pair : this->rooms) {
            room_names.push_back(pair.first);
        }
        std::sort(room_names.begin(), room_names.end());

        for (const std::string& room_name : room_names) {
            const Room& room = this->rooms.at(room_name); // Accéder à la room via son nom
            std::cout << "           " << room.name << " (cap: " << room.capacity << "): [";
            for (size_t i = 0; i < room.antsInside.size(); ++i) {
                std::cout << room.antsInside[i]->id;
                if (i < room.antsInside.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
        }
    }
    std::cout << std::endl; // Ligne vide pour séparer les étapes
}

bool Colony::allArrived() const
{
    if (!this->dormitory) return false; // Sécurité
    for (const Ant &ant_obj : this->ants) // 'ant_obj' pour éviter conflit
    {
        if (!ant_obj.hasArrived(this->dormitory))
        {
            return false;
        }
    }
    return true;
}

void Colony::writeSimulationJSON(const std::string& filename) {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        // Utiliser this->simulation_output_json_ pour accéder au membre de la classe
        outfile << this->simulation_output_json_.dump(4); // .dump(4) pour une sortie formatée
        outfile.close();
        std::cout << "Données de simulation JSON écrites dans " << filename << std::endl;
    } else {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << " pour écriture." << std::endl;
    }
}