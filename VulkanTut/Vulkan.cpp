#include "Vulkan.h"

void Application::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void Application::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vinegar Vulkan", nullptr, nullptr);
}

void Application::initVulkan()
{
	createInstance();

	pickPhysicalDevice();
	createLogicalDevice();
}

void Application::createInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Testing";
	appInfo.pEngineName = "Vinegar";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create instance!");
	}

	
	std::vector<VkExtensionProperties> extensions(glfwExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &glfwExtensionCount, extensions.data());

	std::cout << "Available extensions:" << std::endl;

	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}
}

void Application::pickPhysicalDevice()
{
	physicalDevice = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice = VK_NULL_HANDLE)
	{
		throw std::runtime_error("No Suitable GPUs found!");
	}
}

bool Application::isDeviceSuitable(VkPhysicalDevice device)
{
	//Get Device Properties
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//Get Device Features
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	//Checks the device type, being a Discrete GPU, and if it has the feature of : multiViewport
	bool suitable = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.multiViewport;

	//QueueFamilyIndicies indices = findQueueFamilies(device);

	return suitable;// && indices.isComplete();
}

void Application::createLogicalDevice()
{
	QueueFamilyIndicies indicies = findQueueFamilies(physicalDevice);

	//Device Queue Creation Info, e.g. num of queues, and types
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indicies.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	//Queue Priority
	float quePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &quePriority;

	//Specify used device features
	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pEnabledFeatures = &deviceFeatures;
	deviceInfo.enabledExtensionCount = 0;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	//Validation Layers

	//Create the logical device
	VkResult result = vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &vDevice);
	if (result == !VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}
	
	//Temp Queue stuff
	vkGetDeviceQueue(vDevice, indicies.graphicsFamily.value(), 0, &graphicsQueue);
}

QueueFamilyIndicies Application::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndicies indicies;

	//Get Queue Family Count
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	//Get Queue Families
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	//Find supported queue
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indicies.graphicsFamily = i;
		}

		if (indicies.isComplete())
		{
			break;
		}
	}

	return indicies;
}

void Application::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();


	}
}

void Application::cleanup()
{
	vkDestroyDevice(vDevice, nullptr);
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);

	glfwTerminate();
}
