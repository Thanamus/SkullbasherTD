//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <sre/Renderer.hpp>
#include <utility>
#include "sre/SpriteAtlas.hpp"
#include "Tower.hpp"


#define GLM_ENABLE_EXPERIMENTAL

using namespace sre;
using namespace glm;

Tower::Tower() {}

int Tower::getId() const {
    return id;
}

const std::string &Tower::getName() const {
    return name;
}

const std::shared_ptr<sre::Texture> &Tower::getIcon() const {
    return icon;
}

const std::string &Tower::getModel() const { return model;
}

int Tower::getBuildCost() const {
    return buildCost;
}

float Tower::getConstructionTime() const {
    return constructionTime;
}

float Tower::getDelay() const {
    return delay;
}

float Tower::getLaunchTime() const {
    return launchTime;
}

float Tower::getReloadTime() const {
    return reloadTime;
}

float Tower::getProjectileAirTime() const {
    return projectileAirTime;
}

float Tower::getDamage() const {
    return damage;
}

const vec3 &Tower::getPosition() const {
    return position;
}

const vec3 &Tower::getScale() const {
    return scale;
}

const vec3 &Tower::getRotation() const {
    return rotation;
}

const std::vector<TowerPart> &Tower::getParts() const {
    return parts;
}

const std::string &Tower::getIndicator() const {
    return indicator;
}

void Tower::setId(int id) {
    Tower::id = id;
}

void Tower::setName(const std::string &name) {
    Tower::name = name;
}

void Tower::setIcon(const std::shared_ptr<sre::Texture> &icon) {
    Tower::icon = icon;
}

void Tower::setModel(const std::string &model) {
    Tower::model = model;
}

void Tower::setBuildCost(int buildCost) {
    Tower::buildCost = buildCost;
}

void Tower::setConstructionTime(float constructionTime) {
    Tower::constructionTime = constructionTime;
}

void Tower::setDelay(float delay) {
    Tower::delay = delay;
}

void Tower::setLaunchTime(float launchTime) {
    Tower::launchTime = launchTime;
}

void Tower::setReloadTime(float reloadTime) {
    Tower::reloadTime = reloadTime;
}

void Tower::setProjectileAirTime(float projectileAirTime) {
    Tower::projectileAirTime = projectileAirTime;
}

void Tower::setDamage(float damage) {
    Tower::damage = damage;
}

void Tower::setPosition(const vec3 &position) {
    Tower::position = position;
}

void Tower::setScale(const vec3 &scale) {
    Tower::scale = scale;
}

void Tower::setRotation(const vec3 &rotation) {
    Tower::rotation = rotation;
}

void Tower::setParts(const std::vector<TowerPart> &parts) {
    Tower::parts = parts;
}

void Tower::setIndicator(const std::string &indicator) {
    Tower::indicator = indicator;
}

