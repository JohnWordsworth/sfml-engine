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

	class Scene
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

	private:
		Node m_rootNode;
		std::shared_ptr<CameraNode> m_camera;
        std::unique_ptr<PhysicsWorld> m_physicsWorld;
        std::unique_ptr<SfmlBoxDebugDraw> m_physicsDebug;
        bool m_initialized = false;
	};

} // namespace
