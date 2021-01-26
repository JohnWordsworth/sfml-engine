#include "sfml-engine/cameranode.h"


gbh::CameraNode::CameraNode(sf::RenderTarget* renderTarget) :
    m_renderTarget(renderTarget)
{
    
}


sf::FloatRect gbh::CameraNode::getLocalBounds() const
{
    sf::FloatRect bounds;
    bounds.width = m_renderTarget->getSize().x;
    bounds.height = m_renderTarget->getSize().y;
    
    return bounds;
}
