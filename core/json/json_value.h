#pragma once

#include <sstream>

#include "json_type_base.h"

template<class T>
class JsonValue : public JsonTypeBase
{
private:
    T m_value;

    bool m_is_string_format = true; // Default is true for all std::string

public:
    JsonValue(T value) : m_value(value) {}

    // Inheritance methods
    virtual bool is_json_value() { return true; }
    virtual const std::string get_string_value() const;
    virtual std::shared_ptr<JsonTypeBase> get_clone(const std::shared_ptr<JsonTypeBase>&);

    void update_value(T value) { m_value = value; }
    void add_value(T value) { m_value += value; }
    void sub_value(T value) { m_value -= value; }

    void set_is_string_format(bool val) { m_is_string_format = val; }

    operator T() { return m_value; }
};

template<class T>
std::shared_ptr<JsonTypeBase> JsonValue<T>::get_clone(const std::shared_ptr<JsonTypeBase>& ptr)
{
    return std::make_shared<JsonValue<T>>(m_value);
}

template<class T>
const std::string JsonValue<T>::get_string_value() const
{
    std::stringstream ss;
    std::string res;
    ss << m_value;
    ss >> res;

    return res;
}

