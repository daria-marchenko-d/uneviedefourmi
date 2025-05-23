#include "ants.hpp"
#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>

// ---------- ROOM METHODS ----------

Room::Room(const std::string &name, int capacity)
    : name(name), capacity(capacity) {}

bool Room::isFull() const
{
    return static_cast<int>(antsInside.size()) >= capacity;
}

void Room::addAnt(Ant *ant)
{
    antsInside.push_back(ant);
}

void Room::removeAnt(Ant *ant)
{
    antsInside.erase(std::remove(antsInside.begin(), antsInside.end(), ant), antsInside.end());
}

bool Room::hasNeighbor(Room *room) const
{
    return std::find(neighbors.begin(), neighbors.end(), room) != neighbors.end();
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

void Colony::addRoom(const std::string &name, int capacity)
{
    rooms.emplace(name, Room(name, capacity));
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

void Colony::simulateSteps()
{
    int step =1;
    while (!allArrived())
    {
        std::vector<std::tuple<std::string, std::string, std::string>> current_step_movements;
        std::vector<Ant*>ants_to_move_this_step; // Pour stocker les fourmis qui vont bouger
        std::vector<Room*> next_rooms_for_ants_to_move; // Etleur destinations

        // Phase 1: Identifier les fourmis qui peuvent et vont bouger
        for (size_t i = 0; i < ants.size(); ++i)
        {
            Ant* ant = &ants[i];
            if (ant->hasArrived(dormitory))
            {
                continue;
            }

            Room* next_room_candidate = ant->getNextRoom();
            if (next_room_candidate)
            {
                if (!next_room_candidate->isFull() || next_room_candidate == dormitory)
                {
                    bool room_already_targeted = false;
                    if (next_room_candidate != dormitory) {
                        for(const auto& planned_move_room : next_rooms_for_ants_to_move) {
                            if (planned_move_room == next_room_candidate) {
                                room_already_targeted = true;
                                break;
                            }
                        }
                    }

                    if (!room_already_targeted) {
                        ants_to_move_this_step.push_back(ant);
                        next_rooms_for_ants_to_move.push_back(next_room_candidate);
                    }
                }
            }
        }

        // Phase 2: Effectuer les mouvements identifiés
        if (!ants_to_move_this_step.empty()) {
            for (size_t i = 0; i < ants_to_move_this_step.size(); ++i) {
                Ant* ant = ants_to_move_this_step[i];
                Room* next_room = next_rooms_for_ants_to_move[i];

                Room* old_room = ant->currentRoom;

                if (old_room) {
                    old_room->removeAnt(ant);
                }
                next_room->addAnt(ant);
                ant->advance();

                current_step_movements.emplace_back(ant->id, old_room->name, ant->currentRoom->name);                 
            }
        }

        printStep(step, current_step_movements);
        
        if (ants_to_move_this_step.empty() && !allArrived()) {
            // Gestion basique du blocage ou de l'attente
        }

        step++;
        if (step > 100 && !allArrived()) {
            std::cout << "Simulation arrêtée après 100 étapes (possible blocage)." << std::endl;
            break;
        }
    }
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
    for (const Ant &ant : ants)
    {
        if (!ant.hasArrived(dormitory))
        {
            return false;
        }
    }
    return true;
}