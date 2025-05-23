#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <tuple>

// Represents a room in the ant colony
class Room
{
public:
    std::string name;
    int capacity = 1;
    std::vector<class Ant *> antsInside;
    std::vector<Room *> neighbors;

    Room() = default;
    Room(const std::string &name, int capacity = 1);

    bool isFull() const;
    void addAnt(class Ant *ant);
    void removeAnt(class Ant *ant);
    bool hasNeighbor(Room *room) const;
};


// Represents a single ant
class Ant
{
public:
    std::string id;
    Room *currentRoom = nullptr;
    std::vector<Room *> path;
    int pathIndex = 0;

    Ant() = default;
    Ant(const std::string &id, Room *startRoom);

    bool hasArrived(Room *dormitory) const;
    Room *getNextRoom() const;
    void advance();
};

// Represents the entire ant colony structure and logic
class Colony
{
public:
    std::unordered_map<std::string, Room> rooms;
    std::vector<Ant> ants;
    Room *vestibule = nullptr;
    Room *dormitory = nullptr;

    Colony() = default;

    void addRoom(const std::string &name, int capacity = 1);
    void addTunnel(const std::string &from, const std::string &to);
    void setSpecialRooms(const std::string &vst, const std::string &dor);
    void addAnts(int count);
    void computePaths();
    void simulateSteps();
    void printStep(int stepNumber, const std::vector<std::tuple<std::string, std::string, std::string>>&movements) const;
    bool allArrived() const;
};