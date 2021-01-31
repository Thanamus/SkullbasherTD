#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "Tower.hpp"
#include "architecture/health/CrystalHealth.hpp"

struct enemySetsInWave {
    int enemyType;
    int quantiy;
};

struct waveScheduleDetails {
    int timeBetweenWaves;
    int timeBetweenEnemies;
};

// struct somethingmeaningful { float meaningful1; float meaningful2; };
// using mymeaningfulmap = std::map<int, somethingmeaninful>;
// from https://stackoverflow.com/questions/24882357/stdpair-vs-array

class GameManager
{
public:
    explicit GameManager();

    void init();

    std::vector<std::shared_ptr<Tower>> GetTowers();
    std::shared_ptr<Tower> selectedTower;
    void GameManager::onKey(SDL_Event &event);
    void GameManager::onMouse(SDL_Event &event);
    void TogglePause();
    void ToggleLockMouse();
    bool quit = false;
    bool buildModeActive = false;

    int getScore() const;

    void setScore(int score);

    float getPower() const;

    void setPower(float power);

    bool levelRunning = true;
    bool won = false;
    bool paused = false;

    void updateTowerIndicator();
    std::shared_ptr<class Scene> currentScene;
    std::shared_ptr<class SceneManager> sceneManager;

    void setPath(std::vector<glm::vec3> pathToBe);
    std::vector<glm::vec3> getPath();
    int getFirstPathIndex();
    glm::vec3 getNextPathPoint(int currentPathIndex);

    std::shared_ptr<CrystalHealth> crystal;

    //wave details
    int getCurrentEnemy();
    int getCurrentEnemySet();
    int getCurrentWave();
    int getTotalWavesInLevel();

    int getEnemyAmountWave();
    waveScheduleDetails getCurrentTimeBetweenWaves();

    void setCurrentEnemy(int currentEnemy);
    void addWave(int waveNumber, std::vector<enemySetsInWave> enemySets, waveScheduleDetails waveDetails);

    void updateAllWaveStats();
    void setInitialWaveStats();
    const std::map<int, std::vector<enemySetsInWave>> &getWaveAndEnemys() const;
    int getTotalEnemiesInCurrentSet() const;
private:
    void loadTowers(std::string filename);
    static std::map<std::string, std::shared_ptr<sre::Texture>> inventoryTexture;
    std::vector<std::shared_ptr<Tower>> towers;

    // Player stats
    int score = 42;
    float power = 0.7; // between 0.0 and 1.0

    //path
    std::vector<glm::vec3> path;

//-------------- Wave stats -------------------
    int currentWave = 0;
    int totalWavesInLevel = 0;

    int enemySetsAmount = 0; //assuming this means how many waves


    int currentEnemySet = 0;
    int totalEnemySetsInCurrentWave = 0;
    // int currentEnemyInset = 0;

    int currentEnemy = 0;
    int totalEnemiesInCurrentSet = 0;

    int enemyAmountWave = 0;

    void checkAndUpdateEnemyNumber();
    void checkAndUpdateEnemySetNumber();
    void checkAndUpdateWaveNumber(int tempCurrentEnemyWaveHolder);
    void setTotalEnemiesInCurrentSet(); // made this into a method to maintain consitent code

// struct somethingmeaningful { float meaningful1; float meaningful2; };
// using mymeaningfulmap = std::map<int, somethingmeaninful>;
// from https://stackoverflow.com/questions/24882357/stdpair-vs-array

    std::map <int, std::vector<enemySetsInWave>> waveAndEnemys;
    std::map <int, waveScheduleDetails> waveAndTimeBetweens;
};
