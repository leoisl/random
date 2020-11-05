#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

void skip_to_next_line(char** current_file_pointer) {
    while ( (**current_file_pointer) != '\n')
        ++(*current_file_pointer);
    ++(*current_file_pointer);
}

void skip_n_tabs(char** current_file_pointer, u_int8_t n) {
    u_int8_t number_of_tabs_seen = 0;
    while (number_of_tabs_seen < n) {
        char current_char = **current_file_pointer;
        (*current_file_pointer) += 1;
        number_of_tabs_seen += (current_char=='\t');
    }
}

void skip_until_FILTER_field(char** current_file_pointer) {
    skip_n_tabs(current_file_pointer, 6);
}

void skip_until_FORMAT_field(char** current_file_pointer) {
    skip_n_tabs(current_file_pointer, 2);
}

void skip_until_SAMPLE_field(char** current_file_pointer) {
    skip_n_tabs(current_file_pointer, 1);
}

void check_if_GT_is_in_FORMAT_field(char** current_file_pointer) {
    u_int8_t GT_is_present = (strncmp("GT:", *current_file_pointer, 3)==0 || strncmp("GT\t", *current_file_pointer, 3)==0);
    if (!GT_is_present) {
        fprintf(stderr, "Genotype GT not found in file\n");
        exit(1);
    }
}

void skip_to_next_slash(char** current_file_pointer) {
    while ( (**current_file_pointer) != '/')
        ++(*current_file_pointer);
    ++(*current_file_pointer);
}

void print_GTs(char** current_file_pointer) {
    int gt1 = atoi(*current_file_pointer);
    skip_to_next_slash(current_file_pointer);
    int gt2 = atoi(*current_file_pointer);
    if (gt1 == gt2) {
        printf("%d\n", gt1);
    }
    else {
        printf("%d/%d\n", gt1, gt2);
    }
}


void process_VCF_line(char** current_file_pointer) {
    skip_until_FILTER_field(current_file_pointer);
    u_int8_t passed_filter = strncmp("PASS\t", *current_file_pointer, 5) == 0;
    if (passed_filter) {
        skip_until_FORMAT_field(current_file_pointer);
        check_if_GT_is_in_FORMAT_field(current_file_pointer);
        skip_until_SAMPLE_field(current_file_pointer);
        print_GTs(current_file_pointer);
    }
}

void process_file(char** current_file_pointer, const char* pointer_to_EOF) {
    while ((*current_file_pointer) < pointer_to_EOF) {
        char current_char = **current_file_pointer;
        
        u_int8_t is_comment = (current_char == '#');

        if (!is_comment) {
            process_VCF_line(current_file_pointer);
        }

        skip_to_next_line(current_file_pointer);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: test_read_vcf <in.vcf>\n");
        return 1;
    }

    // based on https://gist.github.com/marcetcheverry/991042
    const char *filepath = argv[1];
    int fd = open(filepath, O_RDONLY, (mode_t)0600);

    if (fd == -1)
    {
        fprintf(stderr, "Error opening VCF file '%s'. Cannot continue", filepath);
        exit(1);
    }

    struct stat fileInfo;
    if (fstat(fd, &fileInfo) == -1)
    {
        fprintf(stderr, "Error getting the file size of '%s'. Cannot continue", filepath);
        exit(1);
    }
    
    char *map = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
    char* current_file_pointer = map;

    process_file(&current_file_pointer, map+fileInfo.st_size);

    // Don't forget to free the mmapped memory
    if (munmap(map, fileInfo.st_size) == -1)
    {
        close(fd);
        fprintf(stderr, "Error un-mmapping the file");
        exit(1);
    }

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);
    
    return 0;
}
