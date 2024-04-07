#include <sys/stat.h>
#include <stdio.h> // remove later
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define HEADER_DELIM "\r\n\r\n"

// get file info
/*
	struct file_info {
		int size;
		char* content;
	}
	- need to call free on `content` and `file_info`
*/

struct file_info {
	long int size;
	char* content;
};

struct file_info* get_file_details(const char* file_name);

int get_size_for_char_ptr(char* c);

// map of `strings` to hold headers
// everything's malloced
struct node {
	char* key;
	char* value;
};

struct map {
	int size;
	struct node **nodes;
	int capcacity;
};

// initialize map
struct map* initialize_map();

// add node to map
void put(struct map* m, char* key, char* value);

// print map
void print_map(struct map* m);

// parse request headers
char** tokenize_req_headers(char* buffer);

// print tokenized req headers
void print_tokenized_req_headers(char** lines);

// struct containing method and requested url
struct method_details {
	char* method;
	char* url;
};

// get method type based on request headers (need to free)
struct method_details* get_method_type(char* first_line);