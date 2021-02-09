//
// Created by kb on 08/02/2021.
//

#include "TowerParser.hpp"
#include "scenes/Scene.hpp"
#include "Model.hpp"
#include "Transform.hpp"

std::vector<std::shared_ptr<Tower>> TowerParser::readTowersFromFile(const std::string& path) {
    using namespace rapidjson;
    std::vector<std::shared_ptr<Tower>> towers {};
    std::ifstream fis(path);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    int rowArrayCount = d["towers"].GetArray().Size();
    for (size_t row = 0; row < rowArrayCount; row++) //go through each 'row' of the map
    {
        const Value& towerRow = d["towers"][row];
        int id = towerRow["id"].GetInt();
        std::string name = towerRow["name"].GetString();
        std::string icon = towerRow["icon"].GetString();
        std::string indicator = towerRow["indicator"].GetString();
        std::string model = towerRow["model"].GetString();
        int buildCost = towerRow["buildCost"].GetInt();
        float constructionTime = towerRow["constructionTime"].GetFloat();
        float delay = towerRow["delay"].GetFloat();
        float range = towerRow["range"].GetFloat();
        float launchTime = towerRow["launchTime"].GetFloat();
        float reloadTime = towerRow["reloadTime"].GetFloat();
        float projectileAirTime = towerRow["projectileAirTime"].GetFloat();
        glm::vec3 position = glm::vec3(towerRow["position"]["x"].GetFloat(),
                                       towerRow["position"]["y"].GetFloat(),
                                       towerRow["position"]["z"].GetFloat());
        glm::vec3 scale = glm::vec3(towerRow["scale"]["x"].GetFloat(),
                                    towerRow["scale"]["y"].GetFloat(),
                                    towerRow["scale"]["z"].GetFloat());
        glm::vec3 rotation = glm::vec3(towerRow["rotation"]["x"].GetFloat(),
                                       towerRow["rotation"]["y"].GetFloat(),
                                       towerRow["rotation"]["z"].GetFloat());
        TowerProjectile projectile;
        projectile.damage = towerRow["projectile"]["damage"].GetFloat();
        projectile.model = towerRow["projectile"]["model"].GetString();
        projectile.mass = towerRow["projectile"]["mass"].GetFloat();
        projectile.position = glm::vec3(towerRow["projectile"]["position"]["x"].GetFloat(),
                                        towerRow["projectile"]["position"]["y"].GetFloat(),
                                        towerRow["projectile"]["position"]["z"].GetFloat());
        projectile.scale = glm::vec3(towerRow["projectile"]["scale"]["x"].GetFloat(),
                                     towerRow["projectile"]["scale"]["y"].GetFloat(),
                                     towerRow["projectile"]["scale"]["z"].GetFloat());
        projectile.rotation = glm::vec3(towerRow["projectile"]["rotation"]["x"].GetFloat(),
                                        towerRow["projectile"]["rotation"]["y"].GetFloat(),
                                        towerRow["projectile"]["rotation"]["z"].GetFloat());
        projectile.hitboxType = towerRow["projectile"]["hitbox"]["type"].GetString();
        if(projectile.hitboxType == "box")
            projectile.hitboxSize = glm::vec3(towerRow["projectile"]["hitbox"]["x"].GetFloat(),
                                            towerRow["projectile"]["hitbox"]["y"].GetFloat(),
                                            towerRow["projectile"]["hitbox"]["z"].GetFloat());
        else if (projectile.hitboxType == "sphere")
            projectile.radius = towerRow["projectile"]["hitbox"]["radius"].GetFloat();
        std::vector<TowerAnimation> animations;
        auto anIter = towerRow.FindMember("animations");
        if(anIter != towerRow.MemberEnd()) {
            int aniRows = towerRow["animations"].GetArray().Size();
            for (size_t aRow = 0; aRow < aniRows; aRow++) {
                TowerAnimation animation{};
                animation.name = towerRow["animations"][aRow]["name"].GetString();
                animation.looping = towerRow["animations"][aRow]["looping"].GetBool();
                auto frIter = towerRow["animations"][aRow].FindMember("frames");
                if(frIter != towerRow.MemberEnd()) {
                    int frRows = towerRow["animations"][aRow]["frames"].GetArray().Size();
                    for (size_t fRow = 0; fRow < frRows; fRow++) {
                        TowerAnimationFrame frame{};
                        frame.duration = towerRow["animations"][aRow]["frames"][fRow]["duration"].GetFloat();
                        frame.translate = glm::vec3(
                                towerRow["animations"][aRow]["frames"][fRow]["translate"]["x"].GetFloat(),
                                towerRow["animations"][aRow]["frames"][fRow]["translate"]["y"].GetFloat(),
                                towerRow["animations"][aRow]["frames"][fRow]["translate"]["z"].GetFloat());
                        frame.scale = glm::vec3(towerRow["animations"][aRow]["frames"][fRow]["scale"]["x"].GetFloat(),
                                                towerRow["animations"][aRow]["frames"][fRow]["scale"]["y"].GetFloat(),
                                                towerRow["animations"][aRow]["frames"][fRow]["scale"]["z"].GetFloat());
                        frame.rotate = glm::vec3(towerRow["animations"][aRow]["frames"][fRow]["rotate"]["x"].GetFloat(),
                                                 towerRow["animations"][aRow]["frames"][fRow]["rotate"]["y"].GetFloat(),
                                                 towerRow["animations"][aRow]["frames"][fRow]["rotate"]["z"].GetFloat());
                        animation.frames.push_back(frame);
                    }
                }
                animations.push_back(animation);
            }
        }
        auto partsIter = towerRow.FindMember("parts");
        std::vector<TowerPart> parts;
        if(partsIter != towerRow.MemberEnd() && partsIter->value.IsArray() && partsIter->value.GetArray().Size() > 0)
            parts = std::move(parseParts(towerRow["parts"]));
        TowerBuilder builder;
        towers.push_back(
                builder
                .withId(id)
                ->withName(name)
                ->withIcon(icon)
                ->withIndicator(indicator)
                ->withModel(model)
                ->withBuildCost(buildCost)
                ->withConstructionTime(constructionTime)
                ->withDelay(delay)
                ->withRange(range)
                ->withLaunchTime(launchTime)
                ->withReloadTime(reloadTime)
                ->withProjectileAirTime(projectileAirTime)
                ->withProjectile(projectile)
                ->withAnimations(animations)
                ->withPosition(position)
                ->withScale(scale)
                ->withRotation(rotation)
                ->withParts(parts)
                ->build());
    }
    return towers;
}

std::vector<TowerPart> TowerParser::parseParts(const Value& partsArray) {
    std::vector<TowerPart> parts {};
    int rowArrayCount = partsArray.GetArray().Size();
    for (size_t row = 0; row < rowArrayCount; row++) {
        TowerPart part;
        const Value& partRow = partsArray[row];
        part.name = partRow["name"].GetString();
        part.model = partRow["model"].GetString();
        part.position = glm::vec3(partRow["position"]["x"].GetFloat(),
                                  partRow["position"]["y"].GetFloat(),
                                  partRow["position"]["z"].GetFloat());
        part.scale = glm::vec3(partRow["scale"]["x"].GetFloat(),
                               partRow["scale"]["y"].GetFloat(),
                               partRow["scale"]["z"].GetFloat());
        part.rotation = glm::vec3(partRow["rotation"]["x"].GetFloat(),
                                  partRow["rotation"]["y"].GetFloat(),
                                  partRow["rotation"]["z"].GetFloat());
        auto anIter = partRow.FindMember("animations");
        if(anIter != partRow.MemberEnd()) {
            int aniRows = partRow["animations"].GetArray().Size();
            for (size_t aRow = 0; aRow < aniRows; aRow++) {
                TowerAnimation animation{};
                animation.name = partRow["animations"][aRow]["name"].GetString();
                animation.looping = partRow["animations"][aRow]["looping"].GetBool();
                auto frIter = partRow["animations"][aRow].FindMember("frames");
                if(frIter != partRow.MemberEnd()) {
                    int frRows = partRow["animations"][aRow]["frames"].GetArray().Size();
                    for (size_t fRow = 0; fRow < frRows; fRow++) {
                        TowerAnimationFrame frame{};
                        frame.duration = partRow["animations"][aRow]["frames"][fRow]["duration"].GetFloat();
                        frame.translate = glm::vec3(
                                partRow["animations"][aRow]["frames"][fRow]["translate"]["x"].GetFloat(),
                                partRow["animations"][aRow]["frames"][fRow]["translate"]["y"].GetFloat(),
                                partRow["animations"][aRow]["frames"][fRow]["translate"]["z"].GetFloat());
                        frame.scale = glm::vec3(partRow["animations"][aRow]["frames"][fRow]["scale"]["x"].GetFloat(),
                                                partRow["animations"][aRow]["frames"][fRow]["scale"]["y"].GetFloat(),
                                                partRow["animations"][aRow]["frames"][fRow]["scale"]["z"].GetFloat());
                        frame.rotate = glm::vec3(partRow["animations"][aRow]["frames"][fRow]["rotate"]["x"].GetFloat(),
                                                 partRow["animations"][aRow]["frames"][fRow]["rotate"]["y"].GetFloat(),
                                                 partRow["animations"][aRow]["frames"][fRow]["rotate"]["z"].GetFloat());
                        animation.frames.push_back(frame);
                    }
                }
                part.animations.push_back(animation);
            }
        }
        std::cout << "read correctly all animations: " << part.animations.size() << std::endl;
        auto partsIter = partRow.FindMember("parts");
        if(partsIter != partRow.MemberEnd() && partsIter->value.IsArray() && partsIter->value.GetArray().Size() > 0)
            part.parts = parseParts(partRow["parts"]);
        parts.push_back(part);
    }
    return parts;
}

std::shared_ptr<GameObject> TowerParser::addTowerToScene(const std::shared_ptr<Tower>& tower, const std::shared_ptr<Scene>& scene) {
    auto model = Model::create().withOBJ(tower->getModel()).build();
    auto towerGO = scene->createGameObject(tower->getName());
    auto towerTR = towerGO->getComponent<Transform>();
    towerTR->position = tower->getPosition();
    towerTR->scale = tower->getScale();
    towerTR->rotation = tower->getRotation();
    for(const auto& p : tower->getParts())
        addPart(towerGO, p);
    auto towerMR = towerGO->addComponent<ModelRenderer>();
    towerMR->setModel(model);
    auto towerAN = towerGO->addComponent<Animator>();
    towerTR->setModelRenderer(towerMR);
    towerTR->setAnimator(towerAN);
    for(const auto& a: tower->getAnimations()) {
        auto animation = std::make_shared<Animation>(a.looping);
        for(const auto& f : a.frames)
            animation->addFrame(f.translate, f.scale, f.rotate, f.duration);
        towerAN->addAnimation(a.name, animation);
    }
    auto towerTB = towerGO->addComponent<TowerBehaviourComponent>();
    towerTB->setReloadTime(tower->getReloadTime());
    towerTB->setRange(tower->getRange());
    towerTB->setProjectileAirTime(tower->getProjectileAirTime());
    towerTB->setLaunchTime(tower->getLaunchTime());
    towerTB->setProjectile(tower->getProjectile());
    towerTB->setEnabled(true);
    return towerGO;
}

void TowerParser::addPart(const std::shared_ptr<GameObject>& parent, const TowerPart& part) {
    auto model = Model::create().withOBJ(part.model).build();
    auto partGO = parent->getScene()->createGameObject(part.name);
    partGO->setParent(parent.get());
    auto partTR = partGO->getComponent<Transform>();
    partTR->position = part.position;
    partTR->scale = part.scale;
    partTR->rotation = part.rotation;
    auto partMR = partGO->addComponent<ModelRenderer>();
    partMR->setModel(model);
    auto partAN = partGO->addComponent<Animator>();
    for(const auto& a: part.animations) {
        auto animation = std::make_shared<Animation>(a.looping);
        for(const auto& f : a.frames)
            animation->addFrame(f.translate, f.scale, f.rotate, f.duration);
        partAN->addAnimation(a.name, animation);
    }
    partTR->setModelRenderer(partMR);
    partTR->setAnimator(partAN);
    for(const auto& p : part.parts) // recursively add parts
        addPart(partGO, p);
}
