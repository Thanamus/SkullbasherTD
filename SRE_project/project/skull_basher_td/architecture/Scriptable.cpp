//
// Created by kb on 18/01/2021.
//

#include <iostream>
#include <sre/Resource.hpp>
#include "Scriptable.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

int my_panic( lua_State* L ) {
    // Do whatever you like.
    std::string err = sol::stack::get<std::string>(L, -1);
    std::cerr << err << std::endl;
    return -1;
}

Scriptable::Scriptable(bool enabled)
        : enabled(enabled) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::package, sol::lib::math);
    lua.set_panic(my_panic);

    //Incomplete implementation of GameObject
    auto gameObject_type = lua.new_usertype<GameObject>( "GameObject",
                                                         "setName", &GameObject::setName,
                                                         "getName", &GameObject::getName,
                                                         "getParent", &GameObject::getParent,
                                                         "getChildren", &GameObject::getChildren
    );

//    auto component_type = lua.new_usertype<Component>("Component",
//                                                      "getGameObject", &Component::getGameObject);

    auto vec3_type = lua.new_usertype<glm::vec3> ("vec3",
                                                  sol::constructors<glm::vec3(float, float, float)>(),
                                                  "x", &glm::vec3::x,
                                                  "y", &glm::vec3::y,
                                                  "z", &glm::vec3::z
    );

    auto vec2_type = lua.new_usertype<glm::vec2> ("vec2",
                                                  sol::constructors<glm::vec2(float, float)>(),
                                                  "x", &glm::vec2::x,
                                                  "y", &glm::vec2::y
    );
}

// loads a script, overwriting an existing one with the same name
bool Scriptable::loadScript(const std::string& name, const std::string& scriptSource, bool isFilename) {
    // creates shared pointer to new script
    auto script = std::make_shared<Script>();

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
        sol::error err = result;
        std::cerr << "FAILED TO LOAD LUA SCRIPT: " << name << "\n";
        std::cerr << err.what() << std::endl;
        // stops function to avoid halting execution completely
        return false;
    }

    script->function = lua[name];
    script->isLoaded = true;
    scripts[name] = script;
    return true;
}

bool Scriptable::isEnabled() const {
    return enabled;
}

void Scriptable::setEnabled(bool _enabled) {
    Scriptable::enabled = _enabled;
}