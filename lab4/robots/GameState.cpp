/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"

#include<iostream>;

GameState::GameState(){}

/*
 * Copy constructor.
 */
GameState::GameState(const GameState &gamestate){
    for(const auto &robot : gamestate.robots) {
        robots.push_back(robot->clone());
    }
    hero = gamestate.getHero();
}

/*
 * Deconstructor.
 */
GameState::~GameState() {
    for(int i = 0; i < robots.size(); i++){
        delete robots[i];
    }
}

/*
 * Overrided = operator.
 */
GameState& GameState::operator =(const GameState &gamestate) {
    if(this != &gamestate) {
        for(Robot* robot: robots){
            delete robot;
        }
        robots.clear();

        for(Robot* newRobot: gamestate.robots){
            robots.push_back(newRobot->clone());
        }

        hero = gamestate.getHero();

        return *this;
    }
}

/*
 * GameState constructor. Creates all the robots on the scene.
 */
GameState::GameState(int numberOfRobots) {
   for (int i = 0; i < numberOfRobots; i++) {
        Robot* robot = new Robot();
        do {robot->teleport();} // random x and y coordinates is generated for the robot.
        while (!isEmpty (*robot)); // Checks so that the unit is clear of robot.
        robots.push_back(robot);
    }
}

/*
 * Draws the scene, robots and hero.
 */
void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    for (size_t i = 0; i < robots.size(); ++i)
        robots[i]->draw(scene);
    hero.draw(scene);
}

/*
 * Teleport the hero to a random x and y coordinate.
 */
void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

/*
 * Moves robots toward the hero.
 */
void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); i++)
        robots[i]->moveTowards (hero);
}

/*
 * Returns the number of destroyed robots.
 */
int GameState::countCollisions() {
    int numberDestroyed = 0;
    unsigned int i = 0;
    while (i < robots.size()) {
        bool collision = (countRobotsAt (*robots[i]) > 1);
        bool hitJunk = junkAt(*robots[i]);

        if ((collision||hitJunk) && robots[i]->getCollisionCount() == 1) {
            if(!hitJunk) robots.push_back(new Junk(*robots[i])); // Two or more robots have collided
            delete robots[i];

            robots[i] = robots[robots.size() - 1];
            robots.pop_back();
            numberDestroyed++;
        } else i++;
    }
    return numberDestroyed;
}

/*
 * Checks to see if there are any robots left. Return false if there are no robots left.
 */
bool GameState::anyRobotsLeft() const {
    for (unsigned int i = 0; i < robots.size(); i++) {
        if(robots[i]->getCollisionCount() == 1) return true; // We found a robot
    }
    return false;
}

/*
 * Checks to see the hero has died or not.
 */
bool GameState::heroDead() const {
    return !isEmpty(hero);
}

/*
 * Checks to see if a specific is safe or not.
 */
bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++)
        if (robots[i]->attacks(unit)) return false;
    if (junkAt(unit)) return false;
    return true;
}

/*
 * Move hero tovards specific unit.
 */
void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}

/*
 * Get hero.
 */
Hero GameState::getHero() const {return hero;}

/*
 * Is Unit empty of robot?
 */
bool GameState::isEmpty(const Unit& unit) const {
    return countRobotsAt(unit) == 0;
}

/*
 * Is there junk at unit?
 */
bool GameState::junkAt(const Unit& unit) const {
    for (size_t i = 0; i < robots.size(); ++i) {
        if (robots[i]->getCollisionCount() == 0 && robots[i]->at(unit)) return true;
    }
    return false;
}

/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robots.size(); ++i) {
        if (robots[i]->getCollisionCount() == 1 && robots[i]->at(unit)) count++;
    }
    return count;
}
