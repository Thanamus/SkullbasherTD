//
// Created by kb on 08/02/2021.
//

#include "Tower.hpp"
#include "sre/Texture.hpp"

class Scene;

class TowerBuilder {
public:

    TowerBuilder(){
        this->reset();
    }

    ~TowerBuilder(){
        tower.reset();
    }

    void reset(){
        tower= std::make_shared<Tower>();
    }

    const TowerBuilder *withId(int id) const {
        tower->setId(id);
        return this;
    }

    const TowerBuilder *withName(std::string name) const {
        tower->setName(name);
        return this;
    }

    const TowerBuilder *withIcon(std::string icon) const {
        auto iconSprite = sre::Texture::create().withFile(icon).withFilterSampling(false).build();
        tower->setIcon(iconSprite);
        return this;
    }

    const TowerBuilder* withModel(std::string model) const {
        tower->setModel(model);
        return this;
    }

    const TowerBuilder* withIndicator(std::string indicator) const {
        tower->setIndicator(indicator);
        return this;
    }

    const TowerBuilder* withBuildCost(int buildCost) const {
        tower->setBuildCost(buildCost);
        return this;
    }

    const TowerBuilder* withConstructionTime(float constructionTime) const {
        tower->setConstructionTime(constructionTime);
        return this;
    }

    const TowerBuilder* withDelay(float delay) const {
        tower->setDelay(delay);
        return this;
    }

    const TowerBuilder* withLaunchTime(float launchTime) const {
        tower->setLaunchTime(launchTime);
        return this;
    }

    const TowerBuilder* withReloadTime(float reloadTime) const {
        tower->setReloadTime(reloadTime);
        return this;
    }

    const TowerBuilder* withProjectileAirTime(float projectileAirTime) const {
        tower->setProjectileAirTime(projectileAirTime);
        return this;
    }

    const TowerBuilder* withPosition(glm::vec3 position) const {
        tower->setPosition(position);
        return this;
    }

    const TowerBuilder* withScale(glm::vec3 scale) const {
        tower->setScale(scale);
        return this;
    }

    const TowerBuilder* withRotation(glm::vec3 rotation) const {
        tower->setRotation(rotation);
        return this;
    }

    const TowerBuilder* withProjectile(Projectile projectile) const {
        tower->setProjectile(projectile);
        return this;
    }

    const TowerBuilder* withParts(const std::vector<TowerPart>& parts) const {
        tower->setParts(parts);
        return this;
    }

    std::shared_ptr<Tower> build() const {
        std::shared_ptr<Tower> result = tower;
        return result;
    }

private:
    std::shared_ptr<Tower> tower;
};
