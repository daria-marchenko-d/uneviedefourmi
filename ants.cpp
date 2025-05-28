#include "ants.hpp"
#include <iostream>
#include <algorithm>

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
    int step = 1;
    while (!allArrived())
    {
        std::vector<std::pair<Ant *, Room *>> movements;

        // Determine possible moves
        for (Ant &ant : ants)
        {
            if (ant.hasArrived(dormitory))
                continue;

            Room *current = ant.currentRoom;
            Room *next = ant.getNextRoom();

            if (next != nullptr &&
                (!next->isFull() || next == dormitory))
            {

                movements.emplace_back(&ant, next);
            }
        }

        // Apply moves
        for (auto &[ant, nextRoom] : movements)
        {
            Room *current = ant->currentRoom;

            current->removeAnt(ant);
            nextRoom->addAnt(ant);

            ant->advance();
        }

        printStep(step);
        step++;
    }
}

void Colony::printStep(int stepNumber) const
{
    std::cout << "+++ E" << stepNumber << " +++\n";
    for (const Ant &ant : ants)
    {
        if (!ant.hasArrived(dormitory))
        {
            Room *next = ant.getNextRoom();
            if (next != nullptr)
            {
                std::cout << ant.id << " - " << ant.currentRoom->name
                          << " - " << next->name << "\n";
            }
        }
    }
    std::cout << std::endl;
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
