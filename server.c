#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h> // need later for multi-threading
#include "utils.h"
#include <string.h>

#define LISTEN_PORT 8000
#define HEADER "HTTP/1.1 200 OK\nServer: Apache/2.2.14 (Win32)\nContent-Type: text/html\n\n"
#define HEADER_SIZE get_size_for_char_ptr(HEADER)
#define GET "GET"

struct client_data {
	int new_socket;
	// struct sockaddr_in client;
};

// function to handle connection
void handle_connection(int new_socket) {
	int data_size = 0;
	sleep(1);
	ioctl(new_socket, FIONREAD, &data_size);
	// printf("Data size received from client = %d\n", data_size);
	char* buffer = (char *) malloc(data_size);
	if (buffer == NULL) {
		fprintf(stderr, "Error creating buffer\n");
		exit(EXIT_FAILURE);
	}
	int bytes_read = read(new_socket, buffer, data_size);
	printf("Bytes read into buffer = %d\n", bytes_read);
	printf("******************data received\n %s\n *********************\n", buffer);
	char* message = "HTTP/1.1 200 OK\nServer: Apache/2.2.14 (Win32)\nContent-Type: text/html\n\n<h1>Hello, World!</h1>";
	printf("writing message = %s\n", message);

	// printf("header size = %d\n", HEADER_SIZE);

	struct file_info* fi = get_file_details("sample.txt");

	int final_buffer_size = HEADER_SIZE + fi->size;

	char* final_buffer = malloc(sizeof(char) * final_buffer_size);

	strncpy(final_buffer, HEADER, HEADER_SIZE);
	strncat(final_buffer, fi->content, fi->size);

	// buffer2 is for parsing
	char* buffer2 = malloc(data_size);
	memcpy(buffer2, buffer, data_size);

	// mapify request
	struct map* m = initialize_map();
	// get request lines
	char** request_lines = tokenize_req_headers(buffer2);
	printf("Tokenized request lines\n");
	// try to convert to map, MAY NOT WORK
	convert_to_map(m, request_lines);
	// print map
	print_map(m);

	// printf("(((((((((((((((((((( tokenize stuff ))))))))))))))))))))\n");
	char** lines = tokenize_req_headers(buffer2);
	//print_tokenized_req_headers(lines);
	// printf("first line before 2nd tokenization: %s\n", lines[0]);
	struct method_details* md = get_method_type(lines[0]);
	// printf("Method: %s\n", md->method);
	// printf("Url: %s\n", md->url);
	free(md->method);
	free(md->url);
	free(md);
	// printf("first line after 2nd tokenizing: %s\n", lines[0]);

	// printf("(((((((((((((((((((( tokenize stuff ))))))))))))))))))))\n");

	int bytes_written = write(new_socket, final_buffer, final_buffer_size - 1);
	printf("Bytes written = %d\n", bytes_written);
	free(final_buffer);
	close(new_socket);
}

struct file_info* parse_request(struct method_details* md) {
	if (md == NULL) {
		fprintf(stderr, "Error, invalid method details struct\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(md->method, GET) == 0) {
		// get method
		
	}
}

void* testThread(void *ptrs) {
	sleep(1);
	struct client_data* cd = (struct client_data*) ptrs;
	printf("New socket = %d\n", cd->new_socket);
	system("ping -c 5 127.0.0.1");
	free(cd);
}

int main() {
	printf("Hello world\n");

	/* thread example
	for(int i=0;i<5;i++) {
		pthread_t t_id;

		struct client_data* cd = (struct client_data*)malloc(sizeof(struct client_data));
		cd->new_socket = i + 1;

		pthread_create(&t_id, NULL, testThread, cd);
		pthread_detach(t_id);
		//pthread_join(t_id, NULL);
		// free(cd);
	}
	*/

	// get socket descriptor;
	int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_descriptor <= 0) {
		fprintf(stderr, "Error Creating socket\n");
		exit(EXIT_FAILURE);
	}

	// set socket options to reuse address for new connections ??
	int option_value = 1; // ??
	int set_socket_options_result = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
	if (set_socket_options_result != 0) {
		fprintf(stderr, "Error Setting socket options\n");
		exit(EXIT_FAILURE);
	}

	// setup sock_addr_in struct
	struct sockaddr_in server;
	struct sockaddr_in client;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(LISTEN_PORT);

	// bind
	int bind_result = bind(socket_descriptor, (struct sockaddr*)&server, sizeof(struct sockaddr));
	if (bind_result != 0) {
		fprintf(stderr, "Error binding socket\n");
		exit(EXIT_FAILURE);
	}

	// listen
	int listen_result = listen(socket_descriptor, 10);
	if (listen_result != 0) {
		fprintf(stderr, "Error listening\n");
		exit(EXIT_FAILURE);
	}

	while(1) {

		// accept connections
		int client_size = sizeof(struct sockaddr);
		int new_socket = accept(socket_descriptor, (struct sockaddr*) &client, (socklen_t*)&client_size);
		if (new_socket <= 0) {
			fprintf(stderr, "Error connecting to client\n");
		}
		handle_connection(new_socket);
	}

	return 0;
}
