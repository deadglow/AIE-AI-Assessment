#include "Behaviour.h"

Behaviour::Behaviour(std::string name)
{
	this->name = name;
}

std::string Behaviour::GetName()
{
	return name;
}

void Behaviour::PrintName()
{
	std::cout << name << std::endl;
}
