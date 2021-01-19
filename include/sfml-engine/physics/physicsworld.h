#pragma once

#include "physicsbody.h"

#include <SFML/System.hpp>
#include <box2d/box2d.h>
#include <memory>
#include <vector>


namespace gbh
{
    class PhysicsWorld
    {
    public:
        PhysicsWorld(const sf::Vector2f gravity);
        ~PhysicsWorld();
        
        std::shared_ptr<PhysicsBody> createEmptyBody();
        std::shared_ptr<PhysicsBody> createBox(const sf::Vector2f& size, const PhysicsMaterial& material = PhysicsMaterial());
        std::shared_ptr<PhysicsBody> createCircle(float radius, const PhysicsMaterial& material = PhysicsMaterial());
        std::shared_ptr<PhysicsBody> createEdgeBox(const sf::Vector2f& size, const PhysicsMaterial& material = PhysicsMaterial());
        
        void simulate(double deltaTime);
        void destroy();
        
        b2World* getBoxWorld() const { return m_boxWorld.get(); }
        float getPixelsPerMeter() const { return m_pixelsPerMeter; }
        
    private:
        std::unique_ptr<b2World> m_boxWorld;
        std::vector<std::shared_ptr<PhysicsBody>> m_bodies;
        float m_pixelsPerMeter = 10.0f;         // Not many pixels per meter as we are dealing with space/vehicles etc
        
        float m_simulatedTime = 0.0f;
        float m_actualTime = 0.0f;
        float m_stepTime = 1.0f/60.0f;
    };
}
