//
// Created by kb on 18/01/2021.
//

#include <iostream>
#include <sre/Resource.hpp>
#include "Scriptable.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Animator.hpp"


// loads a script, overwriting an existing one with the same name
bool Scriptable::loadScript(const std::string& name, const std::string& scriptSource, bool isFilename) {
    // creates shared pointer to new script
    auto script = std::shared_ptr<Script>();

    // if it's a file name, load from file; otherwise use directly the string
    script->data = (isFilename) ? sre::Resource::loadText(scriptSource) : script->data = scriptSource;

    //load the script
    auto result = lua.script(script->data, [](lua_State* L, sol::protected_function_result pfr) {
        // pfr will contain things that went wrong, for either loading or executing the script
        return pfr;}
    );     // evaluate lua script

    script->isLoaded = result.valid();

    // stops if script doesn't load
    if(!script->isLoaded)
    {
        std::cerr << "FAILED TO LOAD LUA SCRIPT: " << name << "\n";
        // stops function to avoid halting execution completely
        return false;
    }

    script->function = lua[name];
    script->isLoaded = true;

    scripts[name] = script;
    return true;
}

Scriptable::Scriptable()
: enabled(true) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::package, sol::lib::math);

    //Incomplete implementation of GameObject
    lua.new_usertype<GameObject>( "GameObject",
                                  "setName", &GameObject::setName,
                                  "getName", &GameObject::getName,
                                  "getParent", &GameObject::getParent,
                                  "getChildren", &GameObject::getChildren
    );

    //Incomplete implementation of Animator
    lua.new_usertype<Animator>( "Animator",
                                "setAnimationState", &Animator::setAnimationState,
                                "getAnimationState", &Animator::getAnimationState
    );

    //Incomplete implementation of Transform
    lua.new_usertype<Transform>( "Transform",
                                "globalTransform", &Transform::globalTransform,
                                "lookAt", sol::overload(
                                        static_cast<void (Transform::*)(glm::vec3, glm::vec3)>(&Transform::lookAt),
                                        static_cast<void (Transform::*)(Transform*, glm::vec3)>(&Transform::lookAt)
                                        )
    );

    auto vec3_type = lua.new_usertype<glm::vec3> ("vec3",
                                                  sol::constructors<glm::vec3(float, float, float)>(),
                                                  "x", &glm::vec3::x,
                                                  "y", &glm::vec3::y,
                                                  "z", &glm::vec3::z
    );
//    //Implementation of getGameObject() from Component base class
//    //This returns a GameObject pointer to the GameObject this component is attached to.
//    lua.set_function("getGameObject", [&]()->GameObject* {
//        return gameObject;
//    });
}

// execute is called with a parameter pack, allowing for scripts to be ran with variable parameters
// since lua doesn't really perform any check on the arguments anyway, as long as the parameters coincide in the script
// this allows to run a generic script which comes in quite handy

// see: https://en.cppreference.com/w/cpp/language/parameter_pack
template<typename ... Args>
void Scriptable::run(const std::string& name, Args ... args) {
    auto script = scripts[name];
    // check if the script is loaded - skips otherwise
    if (script && script->isLoaded) {
        // check if the function is valid - skips otherwise
        if(script->function.valid())
        {
            // attempts to run script with expanded argument pack
            auto result = script->function(&args...);
            // if result is not valid, return error and stop
            if(!result.valid())
            {
                sol::error err = result;
                std::string what = err.what();
                std::cerr << what << std::endl;
            }
        }
    }
}

bool Scriptable::isEnabled() const {
    return enabled;
}

void Scriptable::setEnabled(bool _enabled) {
    Scriptable::enabled = _enabled;
}