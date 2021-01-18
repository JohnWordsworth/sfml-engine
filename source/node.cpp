#include "sfml-engine/node.h"


gbh::Node::~Node() 
{

}


void gbh::Node::addChild(std::shared_ptr<Node> node)
{
	m_children.push_back(node);
	node->m_parent = this;
	node->setScene(m_scene);
}


int gbh::Node::getChildCount() const
{
    return m_children.size();
}


std::shared_ptr<gbh::Node> gbh::Node::getChildAtIndex(int index)
{
    if (index >= 0 && index < m_children.size())
    {
        return m_children[index];
    }
    
    return nullptr;
}


std::shared_ptr<gbh::Node> gbh::Node::getFirstChildWithName(const std::string& name, bool recursive)
{
    for(int i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i]->getName() == name)
        {
            return m_children[i];
        }
    }
    
    if (recursive)
    {
        for(int i = 0; i < m_children.size(); ++i)
        {
            std::shared_ptr<gbh::Node> result = m_children[i]->getFirstChildWithName(name, recursive);
            
            if (result)
            {
                return result;
            }
        }
    }
    
    return nullptr;
}


void gbh::Node::removeChild(int index, bool immediate)
{
    if (immediate)
    {
        m_children.erase(m_children.begin() + index);
    }
    else
    {
        m_children[index]->m_removeInNextUpdate = true;
    }
}


void gbh::Node::removeChildrenWithName(const std::string& name, bool immediate)
{
    std::vector<int> itemsToRemove;
    
    for(int i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i]->getName() == name)
        {
            if (immediate)
            {
                itemsToRemove.push_back(i);
            }
            else
            {
                m_children[i]->m_removeInNextUpdate = true;
            }
        }
    }
    
    // Remove from back, so that all of the indices are still valid
    for(int i = (int)itemsToRemove.size() - 1; i >= 0; ++i)
    {
        m_children.erase(m_children.begin() + i);
    }
}


void gbh::Node::removeFromParent(bool immediate)
{
    if (immediate)
    {
        if (m_parent != nullptr)
        {
            for(int i = 0; i < m_parent->m_children.size(); ++i)
            {
                if (m_parent->m_children[i].get() == this)
                {
                    m_parent->m_children.erase(m_parent->m_children.begin() + i);
                }
            }
        }
    }
    else
    {
        m_removeInNextUpdate = false;
    }
}



void gbh::Node::setName(const std::string& name)
{
	m_name = name;
}


const std::string& gbh::Node::getName() const
{
	return m_name;
}


void gbh::Node::setOrigin(float x, float y)
{
    m_relativeOrigin = sf::Vector2f(x, y);
    updateAbsoluteOrigin();
}


void gbh::Node::setOrigin(const sf::Vector2f& origin)
{
    m_relativeOrigin = origin;
    updateAbsoluteOrigin();
}


const sf::Vector2f& gbh::Node::getOrigin() const
{
    return m_relativeOrigin;
}


gbh::Scene* gbh::Node::getScene()
{
	return m_scene;
}


sf::FloatRect gbh::Node::getLocalBounds() const
{
	return sf::FloatRect();
}


sf::FloatRect gbh::Node::getGlobalBounds() const
{
	sf::Transform globalTransform = getGlobalTransform();
	return globalTransform.transformRect(getLocalBounds());
}


sf::Transform gbh::Node::getGlobalTransform() const
{
	std::vector<sf::Transform> transforms;
	const Node* node = this;

	while (node != nullptr) {
		transforms.push_back(node->getTransform());
		node = node->m_parent;
	}

	sf::Transform transform;

	for (int i = (int)transforms.size() - 1; i >= 0; --i) {
		transform *= transforms[i];
	}

	return transform;
}


std::shared_ptr<gbh::Node> gbh::Node::getNodeAtPoint(const sf::Vector2f& point)
{
	gbh::Node* node = this;
	sf::Transform parentTransform = sf::Transform();
	bool searching = true;

	while (searching) 
	{
		searching = false;

		// Start from the last entry, as we often throw a bunch of nodes on one layer and use 
		// the fact that they render "bottom to top" to layer our graphics. i.e. This will 
		// more likely match the expectations of the user.

		for (int i = (int)node->m_children.size() - 1; i >= 0; --i)
		{
			sf::Transform transform = node->m_children[i]->getTransform() * parentTransform;
			sf::FloatRect bounds = transform.transformRect(node->m_children[i]->getLocalBounds());

			if (bounds.contains(point))
			{
				node = node->m_children[i].get();
				parentTransform = transform;
				searching = true;
				break;
			}
		}
	}

	if (node == this) {
		return nullptr;
	}

	return node->shared_from_this();
}


std::shared_ptr<gbh::Node> gbh::Node::getNodeAtPoint(float x, float y)
{
	return getNodeAtPoint(sf::Vector2f(x, y));
}


void gbh::Node::update(double deltaTime)
{
    // Remove all nodes marked for deletion
    for(int i = (int)m_children.size() - 1; i >= 0; --i)
    {
        if (m_children[i]->m_removeInNextUpdate)
        {
            m_children.erase(m_children.begin() + i);
        }
    }
    
    // Call the 'onUpdate' virtual method
	onUpdate(deltaTime);
	
    // Update all children
	for(std::size_t i = 0; i < m_children.size(); ++i) {
		m_children[i]->update(deltaTime);
	}
}


void gbh::Node::draw(sf::RenderTarget &target, const sf::Transform &parentTransform) const
{
	sf::Transform combinedTransform = parentTransform * getTransform();
	onDraw(target, combinedTransform);
	
	for(std::size_t i = 0; i < m_children.size(); ++i) {
		m_children[i]->draw(target, combinedTransform);
	}
}


void gbh::Node::setScene(gbh::Scene* scene)
{
	m_scene = scene;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->setScene(scene);
	}
}


void gbh::Node::onUpdate(double deltaTime)
{
	// Subclasses of the base node should use this to update themselves
}


void gbh::Node::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	// Subclasses of the base node use this to draw their contents
}


void gbh::Node::updateAbsoluteOrigin()
{
    sf::FloatRect bounds = getLocalBounds();
    sf::Vector2f absoluteOrigin = sf::Vector2f(m_relativeOrigin.x * bounds.width, m_relativeOrigin.y * bounds.height);
    sf::Transformable::setOrigin(absoluteOrigin);
}