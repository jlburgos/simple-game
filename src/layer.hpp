#pragma once

#include <vector>

#include "entity.hpp"

class Layer
{
public:
	Layer() = default;

private:
	std::vector<Entity> entities;
	
};