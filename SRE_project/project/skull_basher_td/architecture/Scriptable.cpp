//
// Created by kb on 18/01/2021.
//

#include <iostream>
#include <sre/Resource.hpp>
#include "Scriptable.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Animator.hpp"

// parts of this class are repurposed from the ScriptComponent used in the 4th assignment, as there was no reason
// to completely reimplement them

bool Scriptable::loadScript(const std::string& name, const std::string& luaScript, bool isFilename) {
    // creates shared pointer to new script
    auto newScript = std::shared_ptr<Script>();

    // if it's a file name, load from file; otherwise use directly the string
    newScript->scriptData = (isFilename) ? sre::Resource::loadText(luaScript) : newScript->scriptData = luaScript;

    //load the script
    auto result = lua.script(newScript->scriptData, [](lua_State* L, sol::protected_function_result pfr) {
        // pfr will contain things that went wrong, for either loading or executing the script
        return pfr;}
    );     // evaluate lua script

    //First check whether the script has been loaded correctly
    if(!result.valid())
    {
        newScript->isLoaded = false;
        //Handle error has occured
        std::cerr << "FAILED TO LOAD LUA SCRIPT: " << name << "\n";

        //Drop out of the init function, to allow the Script to work, until a functional lua script is loaded.
        //This is useful if you're editing your script live.
        return false;
    }

    newScript->function = lua[name];    // get lua function update pos
    //Successfully loaded the script.
    newScript->isLoaded = true;

    // inserts newly loaded script in the scripts vector / or overwrites old one
    scripts[name] = newScript;
    return true;
}

Scriptable::Scriptable(GameObject *gameObject)
: Component(gameObject) {
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

    //Implementation of getGameObject() from Component base class
    //This returns a GameObject pointer to the GameObject this component is attached to.
    lua.set_function("getGameObject", [&]()->GameObject* {
        return gameObject;
    });
}

// run is called with a parameter pack, allowing for scripts to be ran with variable parameters
// since lua doesn't really perform any check on the arguments anyway, as long as the parameters coincide in the script
// this allows to run a generic script which comes in quite handy

// see: https://en.cppreference.com/w/cpp/language/parameter_pack
template<typename ... Args>
void Scriptable::execute(Args ... args) {
    auto func = &Transform::localTransform;
    auto script = currentScript.second;
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

void Scriptable::setEnabled(bool enabled) {
    Scriptable::enabled = enabled;
}

const std::pair<std::string, std::shared_ptr<Scriptable::Script>> &Scriptable::getCurrentScript() const {
    return currentScript;
}

void Scriptable::setCurrentScript(const std::pair<std::string, std::shared_ptr<Script>> &currentScript) {
    Scriptable::currentScript = currentScript;
}
