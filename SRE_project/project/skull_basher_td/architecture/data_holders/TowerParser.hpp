//
// Created by kb on 08/02/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_TOWERFACTORY_HPP
#define SIMPLERENDERENGINEPROJECT_TOWERFACTORY_HPP
#include "TowerBuilder.hpp"
#include "../scenes/Scene.hpp"
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "../components/game_entities/towers/TowerBehaviourComponent.hpp"

#include <fstream>
#include <iostream>
#include <utility>

using namespace rapidjson;

class TowerParser {
public:
    static TowerParser& getInstance() {
        static TowerParser instance;
        return instance;
    }
    TowerParser(TowerParser const&) = delete;
    void operator=(TowerParser const&)  = delete;

    static std::vector<std::shared_ptr<Tower>> readTowersFromFile(const std::string& path);
    static std::shared_ptr<GameObject> addTowerToScene(const std::shared_ptr<Tower>& tower, const std::shared_ptr<Scene>& scene);

private:
    TowerParser() = default;
    static std::vector<TowerPart> parseParts(const Value& partsArray);
    static void addPart(const std::shared_ptr<GameObject>& parent, const TowerPart& part);
};


#endif //SIMPLERENDERENGINEPROJECT_TOWERFACTORY_HPP
