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