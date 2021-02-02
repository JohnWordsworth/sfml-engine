#include "sfml-engine/cameranode.h"

#include "sfml-engine/game.h"

gbh::CameraNode::CameraNode()
{
    m_renderTarget = Game::getInstance().getRenderTarget();
}


sf::FloatRect gbh::CameraNode::getLocalBounds() const
{
    sf::FloatRect bounds;
    
    if (m_renderTarget != nullptr)
    {
        bounds.width = (float)m_renderTarget->getSize().x;
        bounds.height = (float)m_renderTarget->getSize().y;
    }
    
    return bounds;
}
