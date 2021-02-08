//
// Created by kb on 18/01/2021.
//

#include <iostream>
#include <sre/Resource.hpp>
#include "Scriptable.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

Scriptable::Scriptable(bool enabled)
        : enabled(enabled) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::package, sol::lib::math);

    //Incomplete implementation of GameObject
    auto gameObject_type = lua.new_usertype<GameObject>( "GameObject",
                                                         "setName", &GameObject::setName,
                                                         "getName", &GameObject::getName,
                                                         "getParent", &GameObject::getParent,
                                                         "getChildren", &GameObject::getChildren
    );

    auto component_type = lua.new_usertype<Component>("Component",
                                                      "getGameObject", &Component::getGameObject);

    // i'm hella impressed with the guy who developed sol
    auto vec3_type = lua.new_usertype<glm::vec3> ("vec3",
                                                  sol::constructors<glm::vec3(float, float, float)>(),
                                                  sol::meta_function::addition,  sol::resolve<glm::vec<3, float, glm::highp>(glm::vec<3, float, glm::highp> const&, glm::vec<3, float, glm::highp> const&)>(&glm::operator+),
                                                  sol::meta_function::subtraction,  sol::resolve<glm::vec<3, float, glm::highp>(glm::vec<3, float, glm::highp> const&, glm::vec<3, float, glm::highp> const&)>(&glm::operator-),
                                                  sol::meta_function::multiplication, sol::overload(
                                                          sol::resolve<glm::vec<3, float, glm::highp>(glm::vec<3, float, glm::highp> const&, glm::vec<3, float, glm::highp> const&)>(&glm::operator*),
                                                          sol::resolve<glm::vec<3, float, glm::highp>(glm::vec<3, float, glm::highp> const&, float)>(&glm::operator*),
                                                          sol::resolve<glm::vec<3, float, glm::highp>(float, glm::vec<3, float, glm::highp> const&)>(&glm::operator*)),
                                                  "x", &glm::vec3::x,
                                                  "y", &glm::vec3::y,
                                                  "z", &glm::vec3::z,
                                                  "normalize", sol::resolve<glm::vec<3, float, glm::highp>(glm::vec<3, float, glm::highp> const&)>(&glm::normalize),
                                                  "length", sol::resolve<float(glm::vec<3, float, glm::highp> const&)>(&glm::length)
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
    auto result = lua.safe_script(script->data, [](lua_State* L, sol::protected_function_result pfr) {
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
    enabled = _enabled;
}

Scriptable::~Scriptable() {
    lua.collect_garbage();
}
