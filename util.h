#define MAX_CHARS_PER_CMDLINE 2048
#define MAX_WORDS_PER_CMDLINE 256
#define MAX_CHARS_PER_CMD 512
#define MAX_WORDS_PER_CMD 64
#define MAX_ENTRIES_IN_SHELLPATH 256

/** 
 * Modify the shell_paths global. Will run until all MAX_ENTRIES_IN_SHELLPATH
 * elements have been set or a NULL char* is found in newPaths. Returns 1 on
 * success and zero on error *
 */
int set_shell_path(char **newPaths);

/** Returns 1 if this is an absolute path, 0 otherwise */
int is_absolute_path(char *path);

/** Determines whether an executable file with the name `filename` exists in the
 * directory named `dirname`.
 *
 * If so, returns a char* with the full path to the file. This pointer MUST
 * be freed by the calling function.
 *
 * If no such file exists in the directory, or if the file exists but is not
 * executable, this function returns NULL. */
char *exe_exists_in_dir(const char *dirname, const char *filename);
