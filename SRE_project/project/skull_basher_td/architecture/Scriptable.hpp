//
// Created by kb on 18/01/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_SCRIPTABLE_HPP
#define SIMPLERENDERENGINEPROJECT_SCRIPTABLE_HPP

#include <string>
#include <map>
#include "sol.hpp"
#include "Component.hpp"


class Scriptable : public Component{
public:
    struct Script {
        std::string scriptData;
        sol::protected_function function;
        bool isLoaded;
    };

    explicit Scriptable(GameObject *gameObject);

    bool loadScript(const std::string &name, const std::string &luaScript, bool isFilename);

    template<typename ... Args>
    void execute(Args... args);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    const std::pair<std::string, std::shared_ptr<Script>> &getCurrentScript() const;
    void setCurrentScript(const std::pair<std::string, std::shared_ptr<Script>> &currentScript);

private:
    std::map<std::string, std::shared_ptr<Script>> scripts;
    std::pair<std::string, std::shared_ptr<Script>> currentScript;
    GameObject *gameObject = nullptr;

    bool enabled;
    std::string error;
    sol::state lua;
};


#endif //SIMPLERENDERENGINEPROJECT_SCRIPTABLE_HPP
