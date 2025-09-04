#include "layer.hpp"

Layer::Layer() : m_Name("Layer") {}

std::string& Layer::GetName() {
    return m_Name;
}

void Layer::SetName(const std::string& name) {
    m_Name = name;
}
