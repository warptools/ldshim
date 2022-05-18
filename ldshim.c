#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// ld will be located at a relative path of ../lib/ld
const char ld_path[] = {"/../lib/ld"};
// the dynamically linked binary that is shimmed will be located at ../dynbin/<binary name>
const char dynbin_path[] = {"/../dynbin"};

int main(int argc, char *argv[]) {
    int rc;
    char ld[PATH_MAX];
    char self[PATH_MAX];
    char exe[PATH_MAX];
    char exe_path[PATH_MAX];

    // get the absolute path of this binary
    ssize_t self_len = readlink("/proc/self/exe", self, sizeof(self));

    // extract the path of this binary, without the binary name
    strncpy(exe_path, self, sizeof(exe_path));
    char *delim = strrchr(exe_path, '/');
    *delim = '\0';

    // extract the name of this binary
    char *exe_name = strrchr(self, '/');

    // assemble the path to the actual, dynamically linked binary that's being shimmed
    // this is the path of this binary + the relative path to dynbin + the name of this binary
    rc = snprintf(exe, sizeof(exe), "%s%s%s", exe_path, dynbin_path, exe_name);
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
    rc = snprintf(ld, sizeof(exe), "%s%s", exe_path, ld_path);
    if (rc < 0) {
        return 1;
    }

    // run it!
    return execv(ld, new_argv);
}