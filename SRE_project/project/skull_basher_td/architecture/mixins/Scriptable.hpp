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
    explicit Scriptable(bool enabled = false);
    ~Scriptable();

    // run is called with a parameter pack, allowing for scripts to be ran with variable parameters
    // this allows to run a generic script which comes in quite handy

    // see: https://en.cppreference.com/w/cpp/language/parameter_pack

    //template functions must be defined in the header
    template<typename ... Args>
    void run(const std::string &name, Args... args) {
        auto script = scripts[name];
        // check if the script is loaded - skips otherwise
        if (script && script->isLoaded) {
            // check if the function is valid - skips otherwise
            if(script->function.valid())
            {
                // attempts to run script with expanded argument pack
                auto result = script->function(args...);
                // if result is not valid, return error and stop
                if(!result.valid())
                {
                    sol::error err = result;
                    std::string what = err.what();
                    // std::cout << what << std::endl;
                }
            }
        }
    }

    virtual void update(float deltaTime) = 0;

    bool loadScript(const std::string &name, const std::string &scriptSource, bool isFilename);

    bool isEnabled() const;
    void setEnabled(bool _enabled);

protected:
    struct Script {
        std::string data;
        sol::protected_function function;
        bool isLoaded = false;
    };

    sol::state lua;
    std::map<std::string, std::shared_ptr<Script>> scripts;
    bool enabled;
    std::string error;
};


#endif //SIMPLERENDERENGINEPROJECT_SCRIPTABLE_HPP
