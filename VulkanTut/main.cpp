#include "Engine/Engine.h"
#include "Vulkan.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

int main() {
	Application app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}