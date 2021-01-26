#pragma once

#include "node.h"

namespace gbh
{
	class CameraNode : public Node
	{
	public:
		CameraNode(sf::RenderTarget* renderTarget);
        
        virtual sf::FloatRect getLocalBounds() const;
        
    private:
        sf::RenderTarget* m_renderTarget = nullptr;
	};
}
