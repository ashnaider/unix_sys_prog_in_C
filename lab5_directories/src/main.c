#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>

#include <debug.h>

#define PERMS S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH

/* Написать программу, которая удаляет все висячие ссылки в */
/* заданном каталоге и всех его подкаталогах. Имя каталога следует задавать в */
/* виде аргумента командной строки. */

void Chdir(const char* dirname) {
    if (chdir(dirname) == -1) {
        fprintf(stderr, "Error! Can't change dir to \"%s\": %s\n", dirname, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void Fchdir(int fd) {
    if (fchdir(fd) == -1) {
        fprintf(stderr, "Error! Can't change dir to \"%d\": %s\n", fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

DIR* Opendir(const char* dir_name) {
    DIR* dir;
    if ((dir = opendir(dir_name)) == NULL) {
        fprintf(stderr, "Error! Can't open dir \"%s\": %s\n", dir_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return dir;
}

void Closedir(DIR* dir, const char* dir_name) {
    if (closedir(dir) == -1) {
        fprintf(stderr, "Error! Can't close dir \"%s\": %s\n", dir_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void PrintDirContent(DIR* dir) {
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("file type - %d: %s\n", entry->d_type, entry->d_name);
    }
}

bool is_link(const char* name) {
    struct stat buf;
    if (lstat(name, &buf) == -1) {
        fprintf(stderr, "Error! Unable to get lstat for file \"%s\": %s\n", name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return S_ISLNK(buf.st_mode);
}

bool is_broken_link(const char* name) {
    struct stat buf;
    return (stat(name, &buf) == -1);

}

void delete_broken_link(const char* name) {
    if (is_link(name)) {
        if (is_broken_link(name)) {
            if (unlink(name) == -1) {
                fprintf(stderr, "Error! Unable to delete broken link \"%s\": %s", name, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    }
}

char* create_subdir_name(const char dir[], const char dir_entry[]) {
    char* new_path = malloc((strlen(dir) + strlen(dir_entry) + 1) * sizeof(char));

    if (new_path == NULL) {
        fprintf(stderr, "Error! Unable to allocate memory in \"create_subdir_name\": %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    strcpy(new_path, dir);
    strcat(new_path, "/");
    strcat(new_path, dir_entry);

    return new_path;
}

void recursive_parse_dir(const char dir_name[]);

void process_dir_entry(const char* new_path) {
    struct stat buf;
    lstat(new_path, &buf);

    if (S_ISDIR(buf.st_mode)) { // if directory

        recursive_parse_dir(new_path);
    } else {

        delete_broken_link(new_path);
    }
}

void recursive_parse_dir(const char dir_name[]) {
    if (dir_name == NULL || dir_name[0] == '\0') {
        fprintf(stderr, "Error! Empty \"dir_name\" in \"recursive_parse_dir\"\n");
        exit(EXIT_FAILURE);
    }

    debug_print("\n\n");
    debug_print("In dir %s\n", dir_name);

    DIR* dir = Opendir(dir_name);

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, "..") == 0 ||
            strcmp(entry->d_name, "." ) == 0)
            continue;

        debug_print("file type - %d: %s\n", entry->d_type, entry->d_name);

        char* new_path = create_subdir_name(dir_name, entry->d_name);

        process_dir_entry(new_path);

        if (new_path != NULL) {
            free(new_path);
        }
    }

    Closedir(dir, dir_name);
}


int main(int argc, char* argv[]) {

    char* dir_name;
    if (argc < 2) {
        dir_name = ".";
    } else {
        dir_name = argv[1];
    }

    debug_print("Dir name: %s\n", dir_name);

    recursive_parse_dir(dir_name);

    return 0;
}
