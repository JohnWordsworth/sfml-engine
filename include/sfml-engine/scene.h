#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "cameranode.h"
#include "shapenode.h"
#include "spritenode.h"
#include "textnode.h"

//#include "physics/physicsworld.h"
//#include "physics/physicsbody.h"


namespace gbh {

	class CameraNode;

	class Scene : public gbh::Node
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

		virtual void update(sf::Time deltaTime);
		virtual void draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const;

		std::shared_ptr<Node> getNodeAtViewPoint(const sf::Vector2f& point);
		std::shared_ptr<Node> getNodeAtViewPoint(float x, float y);
		
	protected:
		virtual void onUpdate(const sf::Time& deltaTime);
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

		virtual void onInitializeScene();
		virtual void onShowScene();
		virtual void onHideScene();
		virtual void onDestroyScene();

		virtual void onKeyboardEvent(sf::Event& event);
		virtual void onMouseEvent(sf::Event& event);
		virtual void onJoystickEvent(sf::Event& event);

	private:
		std::shared_ptr<CameraNode> m_camera;
		//std::unique_ptr<PhysicsWorld> m_physicsWorld;

		bool m_initialized = false;
	};

} // namespace
