//
// Created by kb on 08/02/2021.
//

#include "TowerBuilder.hpp"

// builder class dedicated to towers
// implemented differently from model to showcase another way to make a builder class
// (also this was a bit easier to write)
    TowerBuilder::TowerBuilder(){
        this->reset();
    }

    TowerBuilder::~TowerBuilder(){
        tower.reset();
    }

    void TowerBuilder::reset(){
        tower= std::make_shared<Tower>();
    }

    const TowerBuilder* TowerBuilder::withId(int id) const {
        tower->setId(id);
        return this;
    }

    const TowerBuilder* TowerBuilder::withName(std::string name) const {
        tower->setName(name);
        return this;
    }

    const TowerBuilder* TowerBuilder::withIcon(std::string icon) const {
        auto iconSprite = sre::Texture::create().withFile(icon).withFilterSampling(false).build();
        tower->setIcon(iconSprite);
        return this;
    }

    const TowerBuilder* TowerBuilder::withModel(std::shared_ptr<Model> model) const {
        tower->setModel(model);
        return this;
    }

    const TowerBuilder* TowerBuilder::withIndicator(std::shared_ptr<Model> indicator) const {
        tower->setIndicator(indicator);
        return this;
    }

    const TowerBuilder* TowerBuilder::withBuildCost(int buildCost) const {
        tower->setBuildCost(buildCost);
        return this;
    }

    const TowerBuilder* TowerBuilder::withConstructionTime(float constructionTime) const {
        tower->setConstructionTime(constructionTime);
        return this;
    }

    const TowerBuilder* TowerBuilder::withDelay(float delay) const {
        tower->setDelay(delay);
        return this;
    }

    const TowerBuilder* TowerBuilder::withLaunchTime(float launchTime) const {
        tower->setLaunchTime(launchTime);
        return this;
    }

    const TowerBuilder* TowerBuilder::withRange(float range) const {
        tower->setRange(range);
        return this;
    }

    const TowerBuilder* TowerBuilder::withReloadTime(float reloadTime) const {
        tower->setReloadTime(reloadTime);
        return this;
    }

    const TowerBuilder* TowerBuilder::withProjectileAirTime(float projectileAirTime) const {
        tower->setProjectileAirTime(projectileAirTime);
        return this;
    }

    const TowerBuilder* TowerBuilder::withPosition(glm::vec3 position) const {
        tower->setPosition(position);
        return this;
    }

    const TowerBuilder* TowerBuilder::withScale(glm::vec3 scale) const {
        tower->setScale(scale);
        return this;
    }

    const TowerBuilder* TowerBuilder::withRotation(glm::vec3 rotation) const {
        tower->setRotation(rotation);
        return this;
    }

    const TowerBuilder* TowerBuilder::withProjectile(TowerProjectile projectile) const {
        tower->setProjectile(projectile);
        return this;
    }

    const TowerBuilder* TowerBuilder::withParts(const std::vector<TowerPart>& parts) const {
        tower->setParts(parts);
        return this;
    }

    const TowerBuilder* TowerBuilder::withAnimations(const std::map<std::string, std::shared_ptr<Animation>>& animations) const {
        tower->setAnimations(animations);
        return this;
    }

    std::shared_ptr<Tower> TowerBuilder::build() const {
        std::shared_ptr<Tower> result = tower;
        return result;
    }

