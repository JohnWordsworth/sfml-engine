#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "cameranode.h"
#include "shapenode.h"
#include "spritenode.h"
#include "textnode.h"

#include "physics/debugdraw.h"
#include "physics/physicsworld.h"
#include "physics/physicsbody.h"


namespace gbh {

	class CameraNode;

	class Scene : public PhysicsContactListener
	{
		friend class gbh::Game;

	public:
		Scene();
		virtual ~Scene();

		void initialize();
		void show();
		void hide();
		void destroy();

		//void createPhysicsWorld(sf::Vector2f gravity);
		//PhysicsWorld* getPhysicsWorld();

		void setCamera(std::shared_ptr<CameraNode> cameraNode);
		std::shared_ptr<CameraNode> getCamera();
		sf::Transform getCameraTransform() const;

		void update(double deltaTime);
		void draw(sf::RenderTarget& target) const;
        
        Node& getOverlay() { return m_overlayNode; }

        void addChild(const std::shared_ptr<gbh::Node>& node);
        int getChildCount() const;
        std::shared_ptr<Node> getChildAtIndex(int index);
        std::shared_ptr<Node> getFirstChildWithName(const std::string& name, bool recursive);
        void removeChild(int index, bool immediate);
        void removeChildrenWithName(const std::string& name, bool immediate);
        
		std::shared_ptr<Node> getNodeAtViewPoint(const sf::Vector2f& point);
		std::shared_ptr<Node> getNodeAtViewPoint(float x, float y);
        
        void createPhysicsWorld(const sf::Vector2f& gravity);
        PhysicsWorld* getPhysicsWorld() const { return m_physicsWorld.get(); }
        void simulatePhysics(double deltaTime);
        
        void setDrawPhysicsDebug(bool value) { m_drawPhysicsDebug = value; }
        bool getDrawPhysicsDebug() const { return m_drawPhysicsDebug; }
        
        void setDebugPhysicsEvents(bool value) { m_debugPhysicsEvents = value; }
        bool getDebugPhysicsEvents() const { return m_debugPhysicsEvents; }
        
        void beginContact(const PhysicsContact& contact) override;
        void endContact(const PhysicsContact& contact) override;
        
        const std::vector<PhysicsContact>& getContactList() const { return m_physicsContacts; };
		
	protected:
		virtual void onUpdate(double deltaTime);
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

		virtual void onInitializeScene();
		virtual void onShowScene();
		virtual void onHideScene();
		virtual void onDestroyScene();

		virtual void onKeyboardEvent(sf::Event& event);
		virtual void onMouseEvent(sf::Event& event);
		virtual void onJoystickEvent(sf::Event& event);
        virtual void onBeginPhysicsContact(const PhysicsContact& contact);
        virtual void onEndPhysicsContact(const PhysicsContact& contact);

	private:
		Node m_rootNode;
        Node m_overlayNode;
        
        std::shared_ptr<CameraNode> m_sceneCamera;
        std::unique_ptr<PhysicsWorld> m_physicsWorld;
        std::unique_ptr<SfmlBoxDebugDraw> m_physicsDebug;
        
        std::vector<PhysicsContact> m_physicsContacts;
    
        bool m_initialized = false;
        bool m_drawPhysicsDebug = false;
        bool m_debugPhysicsEvents = false;
	};

} // namespace
