#include "first_app.hpp"
#include <iostream>

#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"
#include "lve_buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>

// std
#include <stdexcept>
#include <array>
#include <chrono>

namespace lve
{
	struct GlobalUbo
	{
		glm::mat4 projectionView{1.f};
		glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
	};

	FirstApp::FirstApp()
	{
		loadGameObjects();
	}
	FirstApp::~FirstApp() {}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < LveSwapChain::MAX_FRAMES_IN_FLIGHT; i++)
		{
			uboBuffers[i] = std::make_unique<LveBuffer>(
				lveDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				lveDevice.properties.limits.minUniformBufferOffsetAlignment);
			uboBuffers[i]->map();
		}

		SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
		LveCamera camera{};

		auto viewerObject = LveGameObject::createGameObject();
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			// frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = lveRenderer.getAspectRatio();
			// camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				int frameIndex = lveRenderer.getFrameIndex();
				FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};

				// update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");

		auto gameObj = LveGameObject::createGameObject();

		gameObj.model = lveModel;
		gameObj.transform.translation = {0.f, 0.5f, 2.5f};
		gameObj.transform.scale = glm::vec3{3.f, 1.5f, 3.f};
		gameObjects.push_back(std::move(gameObj));
	}
}