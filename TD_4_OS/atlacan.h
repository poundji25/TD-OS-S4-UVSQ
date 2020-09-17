/******************************************************************************/
/*** DO NOT MODIFY THIS FILE **************************************************/
/******************************************************************************/
/*** YOUR PROJECTS WILL BE TESTED/CORRECTED CONSIDERING THE ORIGINAL **********/
/*** VERSION OF THIS FILE, NOT YOURS ******************************************/
/******************************************************************************/

#ifndef _H_ATLACAN_
#define _H_ATLACAN_

#define MAX_FILENAME_SIZE 16

#include <stdbool.h>
#include <stdlib.h>

/** User ID. */
enum atl_uid {
    UID_YOU,                            /**< Your user ID. */
    UID_CLAYTON,                        /**< Clayton ID. */
    UID_OTHER,                          /**< Other ID (non relevant). */
    UID_MAX
};

/** Group ID. */
enum atl_gid {
    GID_EXPLORERS,                      /**< Your group ID. */
    GID_ABITHAN,                        /**< Atlacan inhabitants. */
    GID_PERTONTAN,                      /**< Pertontan tribe. */
    GID_MAX
};

extern enum atl_uid MY_UID;             /**< Your UID alias. */
extern enum atl_gid MY_GID;             /**< Your GID alias. */

/** File stats data structure. */
struct atl_stat {
    char name[MAX_FILENAME_SIZE];       /**< File name. */
    int mode;                           /**< File type + mode. */
    enum atl_uid uid;                   /**< Owner UID. */
    enum atl_gid gid;                   /**< Owner GID. */
    size_t size;                        /**< File size. */
};

/**
 * Atlacan virtual file system initializer.
 * 
 * MUST BE CALLED BEFORE USING THIS LIBRARY.
 * 
 * @return     0 on SUCCESS, other on FAILURE.
 */
int atl_init();

/**
 * Atlacan virtual file system deinitializer.
 * 
 * Called once this library is stopped being used.
 */
void atl_fini();


/**
 * Type checker: regular file.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if a regular file, FALSE else.
 */
bool atl_is_file(const int mode);

/**
 * Type checker: symbolic link.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if a symbolic link, FALSE else.
 */
bool atl_is_link(const int mode);

/**
 * Type checker: directory.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if a directory, FALSE else.
 */
bool atl_is_dir(const int mode);


/**
 * Mode checker: user read.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if UR, FALSE else.
 */
bool atl_is_urmode(const int mode);

/**
 * Mode checker: user write.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if UW, FALSE else.
 */
bool atl_is_uwmode(const int mode);

/**
 * Mode checker: user execution.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if UR, FALSE else.
 */
bool atl_is_uxmode(const int mode);

/**
 * Mode checker: user read.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if GR, FALSE else.
 */
bool atl_is_grmode(const int mode);

/**
 * Mode checker: user write.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if GW, FALSE else.
 */
bool atl_is_gwmode(const int mode);

/**
 * Mode checker: user execution.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if GX, FALSE else.
 */
bool atl_is_gxmode(const int mode);

/**
 * Mode checker: user read.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if OR, FALSE else.
 */
bool atl_is_ormode(const int mode);

/**
 * Mode checker: user write.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if OW, FALSE else.
 */
bool atl_is_owmode(const int mode);

/**
 * Mode checker: user execution.
 *
 * @param[in]  mode  File mode.
 *
 * @return     TRUE if OX, FALSE else.
 */
bool atl_is_oxmode(const int mode);


/**
 * Return the current directory name.
 *
 * @return     Current directory name.
 */
const char *atl_getpwd();

// doit être accessible en exécution

/**
 * Change directory to the given path. 
 * 
 * Target path must allow X mode.
 * This path is either:
 *   - '..' to go to parent directory;
 *   - 'relative path' to go to a direct subdirectory.
 *   
 * YOU CANNOT MAKE MULTIPLE 'CD's IN ONE COMMAND.
 *
 * @param[in]  path  Path to go.
 *
 * @return     0 on SUCCESS, other on FAILURE.
 */
int atl_cd(const char *path);

/**
 * List accessible paths from the current directory.
 *
 * Current directory must allow R mode.
 * paths must be released using atl_free_ls().
 *
 * @param[out] paths      Found paths.
 * @param[out] num_entry  Number of paths in current directory.
 *
 * @return     0 on SUCCESS, other on failure.
 */
int atl_ls(char ***paths, int *num_entry);

/**
 * Release atl_ls() data structure.
 *
 * @param[in]  paths      Paths to release.
 * @param[in]  num_entry  Number of paths to release.
 */
void atl_free_ls(char **paths, const int num_entry);

/**
 * Retrieve file stats.
 *
 * @param[in]  path  Path to stat.
 * @param[out] stat  Stat buffer, filled but NOT allocated.
 *
 * @return     0 on SUCCESS, other on failure.
 */
int atl_stat(const char *path, struct atl_stat *stat);

/**
 * Retrieve file data.
 *
 * @param[in]  path  File to read.
 * @param[out] data  Read data, filled but NOT allocated.
 *
 * @return     0 on SUCCESS, other on failure.
 */
int atl_cat(const char *path, int *data);

#endif