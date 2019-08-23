#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <optional>

struct QueueFamilyIndicies
{
	std::optional<uint32_t> graphicsFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value();
	}
};

class Application
{
public:
	void run();

	//Init
	void initWindow();
	void initVulkan();

	//Create Vulkan VkInstance and set it up
	void createInstance();

	//These functions choose (a) suitable device(s) from those available
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);

	//Logical Device Creation
	void createLogicalDevice();

	//Find suitable queue families
	QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);

	//Main Rendering Loop
	void mainLoop();

	//Destroys resources
	void cleanup();

private:
	//Context
	GLFWwindow* window;

	//Vulkan Objects
	VkInstance instance;				//Vulkan Instance
	VkPhysicalDevice physicalDevice;	//Physical Rendering Device
	VkDevice vDevice;					//Logical Device
	VkQueue graphicsQueue;				//Vulkan queue for graphics

	//Windowing
	//VkSurfaceKHR surface;
};

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;