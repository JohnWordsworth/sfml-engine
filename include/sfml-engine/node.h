#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>


namespace gbh 
{
	// Forward declare some classes we need to know exist, but don't need the details of
	class Scene;
	class Game;
	
	/// <summary>
	/// A scene is made up of a collection of nodes in a hierarchy. Each node can have it's own transformation in the parent space.
	/// Use specialized versions of the Node to add visual effects (SpriteNode for an image, TextNode for text, ShapeNode for an sf::Shape).
	/// </summary>
	class Node : public sf::Transformable, public std::enable_shared_from_this<Node>
	{
		friend class gbh::Game;
		friend class gbh::Scene;
	
	public:
		virtual ~Node();
        
		void addChild(std::shared_ptr<Node> node);
        int getChildCount() const;
        std::shared_ptr<Node> getChildAtIndex(int index);
        std::shared_ptr<Node> getFirstChildWithName(const std::string& name, bool recursive);
        void removeChild(int index, bool immediate);
        void removeChildrenWithName(const std::string& name, bool immediate);
        void removeFromParent(bool immediate);
        
		void setName(const std::string& name);
		const std::string& getName() const;
        
        void setOrigin(float x, float y);
        void setOrigin(const sf::Vector2f& origin);
        const sf::Vector2f& getOrigin() const;
        	
		Scene* getScene();

		//void setPhysicsBody(const std::shared_ptr<PhysicsBody>& body);
		//const std::shared_ptr<PhysicsBody>& getPhysicsBody();

		virtual sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;
		sf::Transform getGlobalTransform() const;

		// Returns the deepest descendant that intersects a point (does not check against this node).
		std::shared_ptr<Node> getNodeAtPoint(const sf::Vector2f& point);
		std::shared_ptr<Node> getNodeAtPoint(float x, float y);

	
	protected:
		void setScene(Scene* scene);

		void update(double deltaTime);
		void draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const;

		virtual void onUpdate(double deltaTime);
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

	
	private:
        void updateAbsoluteOrigin();
        
		std::string m_name;
		Scene* m_scene = nullptr;
		Node* m_parent = nullptr;
        sf::Vector2f m_relativeOrigin;
		std::vector<std::shared_ptr<Node>> m_children;
        bool m_removeInNextUpdate = false;
	};

} // namespace
