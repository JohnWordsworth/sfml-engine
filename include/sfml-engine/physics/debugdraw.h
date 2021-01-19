#pragma once

#include <box2d/box2d.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace gbh
{
    class Scene;

    class SfmlBoxDebugDraw : public b2Draw
    {
    public:
        SfmlBoxDebugDraw(sf::RenderWindow* window, gbh::Scene* scene);
        
        virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
        virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
        virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
        virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
        virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
        virtual void DrawTransform(const b2Transform& xf);
        virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color);
    
    protected:
        sf::Color boxColorToSfml(const b2Color& color, sf::Uint8 alpha = 255);
        sf::Vector2f boxVectorToSfml(const b2Vec2& boxVector, bool scaleToPixels = true);
        
    private:
        sf::RenderWindow* m_window;
        gbh::Scene* m_scene;
        float m_pixelsPerMeter;
    };
}
