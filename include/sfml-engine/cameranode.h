#pragma once

#include "node.h"

namespace gbh
{
	class CameraNode : public Node
	{
	public:
		CameraNode();
        
        virtual sf::FloatRect getLocalBounds() const;
        
    private:
        
        sf::RenderTarget* m_renderTarget = nullptr;
	};
}
