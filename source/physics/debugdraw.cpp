#include "sfml-engine/physics/debugdraw.h"
#include "sfml-engine/scene.h"


gbh::SfmlBoxDebugDraw::SfmlBoxDebugDraw(sf::RenderWindow* window, gbh::Scene* scene) :
    m_window(window), m_scene(scene)
{
    m_pixelsPerMeter = m_scene->getPhysicsWorld()->getPixelsPerMeter();
}


void gbh::SfmlBoxDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape polygon;
    polygon.setPointCount(vertexCount);
    
    for(int i = 0; i < vertexCount; ++i)
    {
        sf::Vector2f point = boxVectorToSfml(vertices[i]);
        polygon.setPoint(i, point);
    }
    
    polygon.setOutlineThickness(1.0f);
    polygon.setOutlineColor(boxColorToSfml(color));
    polygon.setFillColor(sf::Color::Transparent);

    m_window->draw(polygon);
}


void gbh::SfmlBoxDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape polygon;
    polygon.setPointCount(vertexCount);
    
    for(int i = 0; i < vertexCount; ++i)
    {
        sf::Vector2f point = boxVectorToSfml(vertices[i]);
        polygon.setPoint(i, point);
    }
    
    polygon.setOutlineThickness(1.0f);
    polygon.setOutlineColor(boxColorToSfml(color, 64));
    polygon.setFillColor(sf::Color::Transparent);

    m_window->draw(polygon);
}


void gbh::SfmlBoxDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    sf::CircleShape circle;
    circle.setRadius(radius * m_pixelsPerMeter);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(boxVectorToSfml(center));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(1.0f);
    circle.setOutlineColor(boxColorToSfml(color));

    m_window->draw(circle);
}


void gbh::SfmlBoxDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    sf::CircleShape circle;
    circle.setRadius(radius * m_pixelsPerMeter);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(boxVectorToSfml(center));
    circle.setFillColor(boxColorToSfml(color, 64));
    circle.setOutlineThickness(1.0f);
    circle.setOutlineColor(boxColorToSfml(color));

    m_window->draw(circle);

    b2Vec2 endPoint = center + radius * axis;
    DrawSegment(center, endPoint, color);
}


void gbh::SfmlBoxDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    sf::Vertex line[2] =
    {
        sf::Vertex(boxVectorToSfml(p1), boxColorToSfml(color)),
        sf::Vertex(boxVectorToSfml(p2), boxColorToSfml(color))
    };

    m_window->draw(line, 2, sf::Lines);
}


void gbh::SfmlBoxDebugDraw::DrawTransform(const b2Transform& xf)
{
    float lineLength = 0.5;
    b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
    b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
    
    DrawSegment(xf.p, xAxis, b2Color(255, 0, 0));
    DrawSegment(xf.p, yAxis, b2Color(0, 255, 0));
}


void gbh::SfmlBoxDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
    // TODO
}


sf::Color gbh::SfmlBoxDebugDraw::boxColorToSfml(const b2Color& color, sf::Uint8 alpha)
{
    return sf::Color((sf::Uint8)(color.r * 255), (sf::Uint8)(color.g * 255), (sf::Uint8)(color.b * 255), alpha);
}


sf::Vector2f gbh::SfmlBoxDebugDraw::boxVectorToSfml(const b2Vec2& boxVector, bool scaleToPixels)
{
    if (scaleToPixels)
    {
        return sf::Vector2f(boxVector.x * m_pixelsPerMeter, -boxVector.y * m_pixelsPerMeter);
    }
    
    return sf::Vector2f(boxVector.x, boxVector.y);
}
