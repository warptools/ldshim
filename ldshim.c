#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

extern const char *__progname;

const char ld_path[] = {"/../lib/ld"};
const char bin_path[] = {"/real"};

int main(int argc, char *argv[]) {
    int rc;
    char ld[PATH_MAX];
    char self[PATH_MAX];
    char exe[PATH_MAX];
    char exe_path[PATH_MAX];

    // get the absolute path of this binary
    ssize_t self_len = readlink("/proc/self/exe", self, sizeof(self));

    // extract the path of this executable, without the binary name
    strncpy(exe_path, self, sizeof(exe_path));
    char *delim = strrchr(exe_path, '/');
    *delim = '\0';

    // extract the name of this binary
    char *exe_name = strrchr(self, '/');

    // assemble the path to the 'real' binary
    // this is the path of this binary + the prefix + the name of this binary
    rc = snprintf(exe, sizeof(exe), "%s%s%s", exe_path, bin_path, exe_name);
    if (rc < 0) {
        return 1;
    }

    // assemble a new argv 
    char *new_argv[argc+2];

    // by convention, argv[0] is the invocation of this binary
    new_argv[0] = argv[0];

    // argv[1] will be the binary we want to execute with ld
    new_argv[1] = exe;

    // copy the remaining args
    for (int i=1; i < argc; i++) {
        new_argv[i+1] = argv[i];
    }
    new_argv[argc+1] = NULL;

    // assemble the path to our ld 
    // this is the path of this binary + ../lib/ld
    rc = snprintf(ld, sizeof(exe), "%s%s", exe_path, ld_path);
    if (rc < 0) {
        return 1;
    }

    // run it!
    return execv(ld, new_argv);
}