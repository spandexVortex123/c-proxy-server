#include "utils.h"

// returns struct having file size and contents
struct file_info* get_file_details(const char* file_name) {
    if (file_name == NULL) {
        fprintf(stderr, "Invalid file name\n");
        exit(EXIT_FAILURE);
    }

    // check if file is accessible
    if (access(file_name, F_OK) != 0) {
        fprintf(stderr, "Error, file cannot be accessed\n");
        exit(EXIT_FAILURE);
    }

    // allocate stat struct
    struct stat* stat_struct = (struct stat*) malloc(sizeof(struct stat));

    if (stat_struct == NULL) {
        fprintf(stderr, "Error allocating memory for stat struct\n");
        exit(EXIT_FAILURE);
    }

    // call stat
    int stat_call_result = stat(file_name, stat_struct);
    if (stat_call_result != 0) {
        fprintf(stderr, "Error assigning stat struct\n");
        exit(EXIT_FAILURE);
    }

    // get file size
    long int file_size = stat_struct->st_size;

    // free stat struct
    free(stat_struct);

    // create file pointer
    FILE *fp;

    fp = fopen(file_name, "r");

    // create memory for file content buffer
    char* buffer = malloc(file_size);

    if (buffer == NULL) {
        fprintf(stderr, "Error creating buffer for holding file contents\n");
        exit(EXIT_FAILURE);
    }

    // use fread
    size_t bytes_read = fread(buffer, sizeof(char), file_size, fp);
    printf("File size = %ld, bytes read = %ld\n", file_size, bytes_read);

    // create return struct
    struct file_info* fi = malloc(sizeof(struct file_info));

    if (fi == NULL) {
        fprintf(stderr, "Error creating memory for struct file_info\n");
        exit(EXIT_FAILURE);
    }

    fi->size = file_size;
    fi->content = buffer;

    return fi;
}

// returns int value which is the size of char* `c`
int get_size_for_char_ptr(char* c) {
    int i=0;
    while(c[i++] != '\0');
    return i;
}

// initialize map with default size = 0 and lenght of nodes array pointer as `109`
struct map* initialize_map() {
    struct map *m = (struct map*) malloc(sizeof(struct map));
    if (m == NULL) {
        fprintf(stderr, "Error creating map\n");
        exit(EXIT_FAILURE);
    }

    m->size = 0;
    struct node** ns = (struct node**) malloc(sizeof(struct node*) * 100);

    if (ns == NULL) {
        fprintf(stderr, "Error allocating memory for nodes array\n");
        exit(EXIT_FAILURE);
    }

    m->nodes = ns;
    m->size = 0;
    m->capcacity = 100;
    return m;
}

// updates the map pointed by `m` with key `key` and value `value`
void put(struct map* m, char* key, char* value) {
    if (m == NULL) {
        return;
    }

    int key_size = get_size_for_char_ptr(key);
    int value_size = get_size_for_char_ptr(value);
    char* key_b = malloc(key_size);
    char* value_b = malloc(value_size);

    memcpy(key_b, key, key_size);
    memcpy(value_b, value, value_size);

    if (m->size >= 70) {
        // need to update the capacity
    } else {
        if (m->size == 0) {
            // first put action
            struct node* n = malloc(sizeof(struct node));
            if (n == NULL) {
                fprintf(stderr, "Error creating node while `put` action\n");
                exit(EXIT_FAILURE);
            }

            n->key = key_b;
            n->value = value_b;

            m->nodes[0] = n;
            m->size = 1;
        } else {
            // subsequent puts
            int flag = 0;
            for (int i=0;i<m->size;i++) {
                struct node* n = m->nodes[i];
                if(strcmp(n->key, key) == 0) {
                    // update this key's value
                    // free existing value
                    free(n->value);
                    n->value = value_b;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                // new key
                struct node* n = malloc(sizeof(struct node));
                if (n == NULL) {
                    fprintf(stderr, "Error creating node while `put` action\n");
                    exit(EXIT_FAILURE);
                }

                n->key = key_b;
                n->value = value_b;

                m->nodes[m->size] = n;
                m->size += 1;
            }
        }
    }
}

// print map
void print_map(struct map* m) {
    if (m == NULL) return;

    for(int i=0;i<m->size;i++) {
        struct node* n = m->nodes[i];
        printf("Key = %s, Value = %s\n", n->key, n->value);
    }
}

// returns char** containing each header line
// assuming max 100 header lines in request
char** tokenize_req_headers(char* buffer) {
    char** lines = malloc(sizeof(char*) * 100);

    if (lines == NULL) {
        fprintf(stderr, "Error creating lines memory\n");
        exit(EXIT_FAILURE);
    }

    char* line = strtok(buffer, HEADER_DELIM);
    int i=0;
    lines[i++] = line;
    while(line != NULL) {
        line = strtok(NULL, HEADER_DELIM);
        lines[i++] = line;
    }

    return lines;
}

// print tokenized req headers
void print_tokenized_req_headers(char** lines) {
    int i=0;
    while(lines[i] != NULL) {
        printf("this is line from headers =>%s\n", lines[i]);
        i +=1 ;
    }
}

// get method type based on request (need to free)
struct method_details* get_method_type(char* first_line) {
    if (first_line == NULL) {
        fprintf(stderr, "Error reading request data\n");
        exit(EXIT_FAILURE);
    }

    int size = get_size_for_char_ptr(first_line);
    char* buffer = malloc(sizeof(char) * size);
    if (buffer == NULL) {
        fprintf(stderr, "Error creating buffer for copy of first line\n");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, first_line, size);

    char* DELIM = " ";
    char* word = strtok(buffer, DELIM);
    char* word2 = strtok(NULL, DELIM);
    printf("word2: %s\n", word2);
    int word_size1 = get_size_for_char_ptr(word);
    char* word_buffer1 = malloc(sizeof(char) * word_size1);
    if (word_buffer1 == NULL) {
        fprintf(stderr, "Error creating buffer for word\n");
        exit(EXIT_FAILURE);
    }
    memcpy(word_buffer1, word, word_size1);
    int word_size2 = get_size_for_char_ptr(word2);
    char* word_buffer2 = malloc((sizeof(char) * word_size2) - 1); // one less to not include the beginning `/`
    if (word_buffer2 == NULL) {
        fprintf(stderr, "error creating word buffer2\n");
        exit(EXIT_FAILURE);
    }
    memcpy(word_buffer2, word2 + 1, word_size2); // start from 2nd byte (1st index) to exclue beginning `/`
    struct method_details* md = malloc(sizeof(struct method_details));
    memset(md, 0x0, sizeof(struct method_details));
    md->method = word_buffer1;
    md->url =  word_buffer2;
    free(buffer);
    return md;
}

// returns exactly size of key, need to add +1 for null byte
int get_key_size(char* line) {
	printf("inside get key size function\n");
	int i=0;
	while(line[i] != '\0') {
		if (line[i] == ':') {
			return i;
		}
		i += 1;
	}
	return -1;
}

// copy value
void copy_value(char* line, char* buffer, int start, int end) {
	printf("in copy_value start: %d\n", start);
	printf("in copy_value end: %d\n", end);

	printf("buffer before update: %s\n", buffer);

	/*	
	while(start < end) {
		printf("char = %c\n", ((char)line[start]));
		buffer[start] = line[start];
		printf("char = %c\n", ((char)buffer[start]));
		start += 1;
	}
	buffer[start] = '\0';
	*/

	int i =0;
	printf("----------------------debug---------------------\n");
	while(buffer[i] != '\0') {
		printf("0x%x\n", buffer[i]);
	}
	printf("----------------------debug---------------------\n");

}

// convert request lines to map
void convert_to_map(struct map* m, char** request_lines) {
	if (m == NULL || request_lines == NULL) {
		fprintf(stderr, "Error m or request_lines NULL\n");
		exit(EXIT_FAILURE);
	}

	// lines loop
	int i = 0;
	while(request_lines[i] != NULL) {
		char* line = request_lines[i];
		int key_size = get_key_size(line);
		if (key_size > 0) {
			// allocate memory
			char* key_buffer = malloc((sizeof(char) * key_size) + 2);
			if (key_buffer == NULL) {
				fprintf(stderr, "Error creating buffer for key\n");
				exit(EXIT_FAILURE);
			}
			// copy key
			memcpy(key_buffer, line, key_size);
			key_buffer[key_size] = '\0';
			// get value
			int value_size = get_size_for_char_ptr(line) - (key_size + 2);
			printf("Value size: %d\n", value_size);
			if (value_size > 0) {
				char* value_buffer = malloc(sizeof(char) * value_size);
				if (value_buffer == NULL) {
					fprintf(stderr, "error creating buffer for value\n");
					exit(EXIT_FAILURE);
				}
				// memcpy(line + key_size + 2, value_buffer, value_size);
				copy_value(line, value_buffer, key_size + 2, get_size_for_char_ptr(line));
				printf("Value pointer: %p\n", value_buffer);
				printf("Value: %s\n", value_buffer);
				put(m, key_buffer, value_buffer);
			}
		}
		i += 1;
	}
}












