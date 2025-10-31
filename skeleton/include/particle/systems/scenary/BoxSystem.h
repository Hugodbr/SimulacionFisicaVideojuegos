#pragma once

#include "GridSystem.h"

class BoxSystem : public GridSystem
{
public:
    BoxSystem(const Region &region, float pointSize = 1.0f, double scale = 10.0, const Vector4& color = Constants::Color::White);
    ~BoxSystem() = default;

    void init() override;
    void update(double deltaTime) override;

protected:
	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
        return Constants::System::Grid::ReserveCountPerGenerator;
    }

    virtual void initParticleGeneratorAndPool() override;

    virtual void createParticlesInGrid() override;
    
    MeshData _meshData;
};