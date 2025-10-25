#include "ForceGenerator.h"


// Initialize static member
uint64_t ForceGenerator::_nextId = 0;

ForceGenerator::ForceGenerator()
    : _id(_nextId++)
{
}

uint64_t ForceGenerator::getId() const
 { 
    return _id; 
}