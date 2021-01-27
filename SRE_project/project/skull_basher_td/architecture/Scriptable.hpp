//
// Created by kb on 18/01/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_SCRIPTABLE_HPP
#define SIMPLERENDERENGINEPROJECT_SCRIPTABLE_HPP

#include <string>
#include <map>
#include "sol.hpp"
#include "Updatable.hpp"


class Scriptable {
public:
    explicit Scriptable();

    template<typename ... Args>
    void run(const std::string &name, Args... args);

    virtual void update() = 0;

    bool loadScript(const std::string &name, const std::string &scriptSource, bool isFilename);

    bool isEnabled() const;
    void setEnabled(bool _enabled);

protected:
    struct Script {
        std::string data;
        sol::function function;
        bool isLoaded = false;
    };

    sol::state lua;

private:
    std::map<std::string, std::shared_ptr<Script>> scripts;
    bool enabled;
    std::string error;
};


#endif //SIMPLERENDERENGINEPROJECT_SCRIPTABLE_HPP
