//
// Created by kb on 08/02/2021.
//

#include "Tower.hpp"
#include "sre/Texture.hpp"

class Scene;

// builder class dedicated to towers
// implemented differently from model to showcase another way to make a builder class
class TowerBuilder {
public:
    TowerBuilder();
    ~TowerBuilder();

    void reset();

    const TowerBuilder *withId(int id) const;

    const TowerBuilder *withName(std::string name) const;
    const TowerBuilder *withIcon(std::string icon) const;
    const TowerBuilder* withModel(std::shared_ptr<Model> model) const;
    const TowerBuilder* withIndicator(std::shared_ptr<Model> indicator) const;
    const TowerBuilder* withBuildCost(int buildCost) const;
    const TowerBuilder* withConstructionTime(float constructionTime) const;
    const TowerBuilder* withDelay(float delay) const;
    const TowerBuilder* withLaunchTime(float launchTime) const;
    const TowerBuilder* withRange(float range) const;
    const TowerBuilder* withReloadTime(float reloadTime) const;
    const TowerBuilder* withProjectileAirTime(float projectileAirTime) const;
    const TowerBuilder* withPosition(glm::vec3 position) const;
    const TowerBuilder* withScale(glm::vec3 scale) const;
    const TowerBuilder* withRotation(glm::vec3 rotation) const;
    const TowerBuilder* withProjectile(TowerProjectile projectile) const;
    const TowerBuilder* withParts(const std::vector<TowerPart>& parts) const ;
    const TowerBuilder* withAnimations(const std::map<std::string, std::shared_ptr<Animation>>& animations) const;
    std::shared_ptr<Tower> build() const;

private:
    std::shared_ptr<Tower> tower;
};
