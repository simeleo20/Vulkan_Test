#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"

// std
#include <memory>
#include <vector>

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp &) = delete;
		FirstApp &operator=(const FirstApp &) = delete;

		void run();

	private:
		void loadGameObjects();

		LveWindow lveWindow{WIDTH, HEIGHT, "hello vulkan!"};
		LveDevice lveDevice{lveWindow};
		LveRenderer lveRenderer{lveWindow, lveDevice};

		std::vector<LveGameObject> gameObjects;
	};
}