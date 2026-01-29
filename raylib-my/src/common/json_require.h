#pragma once

#include <exception>
#include <string>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace JsonRequire {
  template <typename ThrowFn>
  const nlohmann::json& Object(const nlohmann::json& j, const char* name, ThrowFn&& throw_fn) {
    if (!j.contains(name) || !j.at(name).is_object()) {
      throw_fn(std::string("Missing or invalid object: ") + name);
      throw std::runtime_error("JsonRequire::Object: throw_fn did not throw");
    }
    return j.at(name);
  }

  template <typename T, typename ThrowFn>
  T Field(const nlohmann::json& j, const char* name, ThrowFn&& throw_fn) {
    if (!j.contains(name)) {
      throw_fn(std::string("Missing required field: ") + name);
      throw std::runtime_error("JsonRequire::Field: throw_fn did not throw");
    }

    try {
      return j.at(name).get<T>();
    } catch (const std::exception& ex) {
      throw_fn(std::string("Invalid field type: ") + name + ": " + ex.what());
      throw std::runtime_error("JsonRequire::Field: throw_fn did not throw");
    }
  }
}
