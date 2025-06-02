#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <tuple>
#include "json.hpp"

using json = nlohmann::json;

// Represents a room in the ant colony
class Room
{
public:
    std::string name;
    int capacity = 1;
    std::vector<class Ant *> antsInside;
    std::vector<Room *> neighbors;

    int x = 0;
    int y =0;
    int width = 0;
    int height = 0;

    Room() = default;
    Room(const std::string &name, int capacity = 1, int vis_x = 0, int vis_y = 0, int vis_w = 0, int vis_h = 0);

    bool isFull() const;
    void addAnt(class Ant *ant);
    void removeAnt(class Ant *ant);
    bool hasNeighbor(Room *room) const;

    int getCurrentAntsCount() const {
        return antsInside.size();
    }
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

private:
    json simulation_output_json_; //Pour stoquer toutes les données de simulation

public:
    std::unordered_map<std::string, Room> rooms;
    std::vector<Ant> ants;
    Room *vestibule = nullptr;
    Room *dormitory = nullptr;

    Colony() = default;

    void addRoom(const std::string &name, int capacity, int x, int y, int width, int height);
    void addRoom(const std::string &name, int x, int y, int width, int height); // Surcharge

    void addTunnel(const std::string &from, const std::string &to);
    void setSpecialRooms(const std::string &vst, const std::string &dor);
    void addAnts(int count);
    void computePaths();

    void simulateSteps(int max_steps = 100);

    void printStep(int stepNumber, const std::vector<std::tuple<std::string, std::string, std::string>>&movements) const;
    bool allArrived() const;

    void writeSimulationJSON(const std::string& filename); 
};