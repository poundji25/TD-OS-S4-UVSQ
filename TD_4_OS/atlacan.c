/******************************************************************************/
/*** DO NOT MODIFY THIS FILE **************************************************/
/******************************************************************************/
/*** YOUR PROJECTS WILL BE TESTED/CORRECTED CONSIDERING THE ORIGINAL **********/
/*** VERSION OF THIS FILE, NOT YOURS ******************************************/
/******************************************************************************/

#include "atlacan.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

/******************************************************************************/
/*** STATIC DEFINES ***********************************************************/
/******************************************************************************/

#define MAX(a, b) ((a) > (b)) ? (a) : (b);

#define NUM_FSENTRY 64

#define AVG_NUMFLS  5

#define RND_DIR     25
#define RND_LINK    5

#define RND_CLAYTON 10
#define RND_OTHERS  75

#define RND_ABITHAN 60
#define RND_PERTONT 20

/******************************************************************************/
/*** STATIC STRUCTURES ********************************************************/
/******************************************************************************/

enum fs_type {
    FS_INVAL,
    FS_FILE,
    FS_DIR,
    FS_LINK,
    FS_MAX
};

// 0 is root, and no sibling so if sibling_id or child_id = 0, last entry
struct fs_entry {
    int fs_id;
    int parent_id;

    char *name;
    enum fs_type type;
    int data;
    int mode;
    enum atl_uid uid;
    enum atl_gid gid;
    size_t size;

    int num_child;
    int sibling_id;
    int child_id;
};

static struct fs_entry *FS_TREE;
static int FS_PWD_ID;
static int TREASURE_ID;

enum atl_uid MY_UID = UID_YOU;
enum atl_gid MY_GID = GID_EXPLORERS;

/******************************************************************************/
/*** STATIC FUNCTIONS *********************************************************/
/******************************************************************************/

static void _exit_atlacan()
{
    atl_fini();
    printf("You will never leave the jungle\n");
    exit(0);    
}

void _enter_atlacan()
{
    atl_fini();
    printf("You finally found Atlacan\n");
    exit(0);
}

bool _authorize_r_access(const int id)
{
    if (FS_TREE[id].uid == MY_UID)      return FS_TREE[id].mode & 0400;
    if (FS_TREE[id].gid == MY_GID)      return FS_TREE[id].mode & 0040;
                                        return FS_TREE[id].mode & 0004;
}

bool _authorize_x_access(const int id)
{
    if (FS_TREE[id].uid == MY_UID)      return FS_TREE[id].mode & 0100;
    if (FS_TREE[id].gid == MY_GID)      return FS_TREE[id].mode & 0010;
                                        return FS_TREE[id].mode & 0001;
}

char *_gen_name()
{ return strrchr(tmpnam(NULL), '/') + 1; }

enum fs_type _gen_type()
{
    int a = rand() % 100;

    if (a < RND_DIR)                    return FS_DIR;
    if (a < RND_DIR + RND_LINK)         return FS_LINK;
                                        return FS_FILE;
}

enum atl_uid _gen_uid()
{
    int a = rand() % 100;

    if (a < RND_CLAYTON)                return UID_CLAYTON;
    if (a < RND_CLAYTON + RND_OTHERS)   return UID_OTHER;
                                        return UID_YOU;
}

enum atl_gid _gen_gid()
{
    int a = rand() % 100;

    if (a < RND_ABITHAN)                return GID_ABITHAN;
    if (a < RND_ABITHAN + RND_PERTONT)  return GID_PERTONTAN;
                                        return GID_EXPLORERS;
}

static int _init_fil(const int fs_id, const int parent_id,
                      const char *name, const int mode, const enum fs_type type,
                      const enum atl_uid uid, const enum atl_gid gid)
{
    FS_TREE[fs_id].fs_id =      fs_id;
    FS_TREE[fs_id].parent_id =  parent_id;

    FS_TREE[fs_id].name =       name ? strdup(name) : _gen_name();
    FS_TREE[fs_id].type =       type;
    FS_TREE[fs_id].data =       -1;
    FS_TREE[fs_id].mode =       mode == -1 ? FS_TREE[parent_id].mode : mode;
    FS_TREE[fs_id].uid =        uid;
    FS_TREE[fs_id].gid =        gid;
    FS_TREE[fs_id].size =       4;

    if (!FS_TREE[fs_id].name) {
        fprintf(stderr, "alloc error");
        return 1;
    }

    return 0;
}

static void _make_accessible_path(const int i)
{
    if (!i)
        return;

    if (FS_TREE[i].uid == UID_CLAYTON)
        FS_TREE[i].uid = UID_YOU;

    if (FS_TREE[i].gid == GID_PERTONTAN)
        FS_TREE[i].gid = GID_ABITHAN;

    if (FS_TREE[i].uid == UID_YOU)              FS_TREE[i].mode |= 0100;
    else if (FS_TREE[i].gid == GID_EXPLORERS)   FS_TREE[i].mode |= 010;
    else                                        FS_TREE[i].mode |= 01;

    _make_accessible_path(FS_TREE[i].parent_id);
}

static void _fill_data(const int target, const int prev, const bool down,
                       const int nb_steps)
{
    int i;

    if (target < 0)
        return;

    for (i = FS_TREE[target].child_id; i; i = FS_TREE[i].sibling_id) {
        if (!down && prev == i)         continue;

        if (FS_TREE[i].type == FS_FILE) {
            FS_TREE[i].data = nb_steps;
            if (FS_TREE[i].gid == GID_PERTONTAN)
                FS_TREE[i].data = MAX(FS_TREE[i].data - 2, 0);
        }

        if (FS_TREE[i].type == FS_DIR)
            _fill_data(i, target, true, nb_steps + 1);
    }

    if (!down)
        _fill_data(FS_TREE[target].parent_id, target, false, nb_steps + 1);
}

static void _init_data()
{
    int i;

    while (1) {
        i = rand() % NUM_FSENTRY;

        if (FS_TREE[i].type == FS_FILE)
            break;
    }

    TREASURE_ID = i;

    FS_TREE[i].size = 42 * 1024 * 1024;
    FS_TREE[i].uid = UID_OTHER;
    FS_TREE[i].gid = GID_ABITHAN;
    FS_TREE[i].mode |= 04;
    FS_TREE[i].data = 0;

    _make_accessible_path(FS_TREE[i].parent_id);
    _fill_data(FS_TREE[i].parent_id, i, false, 0);
}

static int _gen_level(const int *ids, const int nb_id)
{
    static int fs_id = 1;

    int first_id = fs_id;
    int * next_ids;
    int nb_dir = 0;
    int i, j;
    int rc;

    for (i = 0; i < nb_id; ++i) {
        int nb = AVG_NUMFLS;
        int k = nb;

        FS_TREE[ids[i]].child_id = (fs_id < NUM_FSENTRY) ? fs_id : 0;

        if (!i && fs_id < NUM_FSENTRY) {
            rc = _init_fil(fs_id++, ids[i], _gen_name(), FS_TREE[ids[i]].mode,
                           FS_DIR, _gen_uid(), _gen_gid());
            if (rc)
                return rc;

            FS_TREE[fs_id - 1].sibling_id = (k - 1) ? fs_id : 0;
            ++FS_TREE[ids[i]].num_child;

            --k;
            ++nb_dir;
        }

        while (k) {
            if (fs_id >= NUM_FSENTRY)
                break;

            rc = _init_fil(fs_id++, ids[i], _gen_name(), FS_TREE[ids[i]].mode,
                           _gen_type(), _gen_uid(), _gen_gid());
            if (rc)
                return rc;

            if (k - 1)
                FS_TREE[fs_id - 1].sibling_id = (fs_id < NUM_FSENTRY) ? fs_id : 0;

            ++FS_TREE[ids[i]].num_child;

            if (FS_TREE[fs_id - 1].type == FS_DIR)
                ++nb_dir;

            --k;
        }
    }

    if (fs_id >= NUM_FSENTRY)
        return 0;

    next_ids = malloc(nb_dir * sizeof(*next_ids));

    for (i = first_id, j = 0; j != nb_dir; ++i) {
        if (FS_TREE[i].type != FS_DIR)
            continue;

        next_ids[j++] = i;
    }

    if (nb_dir) {
        rc = _gen_level(next_ids, nb_dir);
    }

    free(next_ids);

    return rc;
}

/******************************************************************************/
/*** LIBRARY FUNCTIONS ********************************************************/
/******************************************************************************/

bool atl_is_file(const int mode) { return ((07000 & mode) >> 9) == FS_FILE; }
bool atl_is_link(const int mode) { return ((07000 & mode) >> 9) == FS_LINK; }
bool atl_is_dir(const int mode)  { return ((07000 & mode) >> 9) == FS_DIR; }

bool atl_is_urmode(const int mode) { return (0400 & mode); }
bool atl_is_uwmode(const int mode) { return (0200 & mode); }
bool atl_is_uxmode(const int mode) { return (0100 & mode); }
bool atl_is_grmode(const int mode) { return (0040 & mode); }
bool atl_is_gwmode(const int mode) { return (0020 & mode); }
bool atl_is_gxmode(const int mode) { return (0010 & mode); }
bool atl_is_ormode(const int mode) { return (0004 & mode); }
bool atl_is_owmode(const int mode) { return (0002 & mode); }
bool atl_is_oxmode(const int mode) { return (0001 & mode); }

const char *atl_getpwd() { return FS_TREE[FS_PWD_ID].name; }

int atl_cd(const char *path) {
    int i;

    if (!path) {
        fprintf(stderr, "path is invalid\n");
        return 1;
    }

    if (!strcmp(path, "..")) {
        FS_PWD_ID = FS_TREE[FS_PWD_ID].parent_id;
        return 0;
    }

    for (i = FS_TREE[FS_PWD_ID].child_id;
         i && strcmp(path, FS_TREE[i].name);
         i = FS_TREE[i].sibling_id);

    if (!i) {
        fprintf(stderr, "path '%s' does not exist\n", path);
        return 1;
    }

    if (FS_TREE[i].type != FS_DIR) {
        fprintf(stderr, "path '%s' is not a directory\n", path);
        return 1;
    }

    if (!_authorize_x_access(FS_PWD_ID))
        _exit_atlacan();

    FS_PWD_ID = i;

    return 0;
}

int atl_ls(char ***paths, int *num_entry) {
    int i, j;

    if (!paths) {
        fprintf(stderr, "paths is invalid\n");
        return 1;
    }

    if (!num_entry) {
        fprintf(stderr, "num_entry is invalid\n");
        return 1;
    }

    if (!_authorize_r_access(FS_PWD_ID))
        _exit_atlacan();

    *paths = malloc(FS_TREE[FS_PWD_ID].num_child * sizeof(**paths));
    if (!*paths) {
        fprintf(stderr, "alloc error\n");
        return 1;
    }

    for (i = FS_TREE[FS_PWD_ID].child_id, j = 0; i;
         i = FS_TREE[i].sibling_id, ++j) {
        (*paths)[j] = strdup(FS_TREE[i].name);

        if (!(*paths)[j]) {
            fprintf(stderr, "alloc error\n");
            for (--j; j >= 0; --j)
                free((*paths)[j]);
            free(*paths);
            return 1;
        }
    }

    *num_entry = FS_TREE[FS_PWD_ID].num_child;

    return 0;
}

void atl_free_ls(char **paths, const int num_entry) {
    int i;

    for (i = 0; i < num_entry; ++i)
        free(paths[i]);
    free(paths);
}

int atl_stat(const char *path, struct atl_stat *stat) {
    int i;

    if (!path) {
        fprintf(stderr, "path is invalid\n");
        return 1;
    }

    if (!stat) {
        fprintf(stderr, "stat is invalid\n");
        return 1;
    }

    if (!strcmp(path, "."))
        i = FS_PWD_ID;
    else {
        for (i = FS_TREE[FS_PWD_ID].child_id;
             i && strcmp(path, FS_TREE[i].name);
             i = FS_TREE[i].sibling_id);

        if (!i) {
            fprintf(stderr, "path '%s' does not exist\n", path);
            return 1;
        }
    }

    strcpy(stat->name, FS_TREE[i].name);
    stat->mode = FS_TREE[i].type * 01000 +
                 FS_TREE[i].mode;
    stat->uid = FS_TREE[i].uid;
    stat->gid = FS_TREE[i].gid;
    stat->size = FS_TREE[i].size;

    return 0;   
}

int atl_cat(const char *path, int *data) {
    int i;

    if (!path) {
        fprintf(stderr, "path is invalid\n");
        return 1;
    }

    for (i = FS_TREE[FS_PWD_ID].child_id;
         i && strcmp(path, FS_TREE[i].name);
         i = FS_TREE[i].sibling_id);

    if (!i) {
        fprintf(stderr, "path '%s' does not exist\n", path);
        return 1;
    }

    if (i == TREASURE_ID)
        _enter_atlacan();

    if (FS_TREE[i].type == FS_LINK)
        _exit_atlacan();

    if (FS_TREE[i].type == FS_DIR) {
        fprintf(stderr, "path '%s' is not a regular file\n", path);
        return 1;
    }

    if (!_authorize_r_access(FS_PWD_ID))
        _exit_atlacan();

    *data = FS_TREE[i].data;

    return 0;
}

int atl_init() {
    int id = 0;
    int rc;

    FS_TREE = calloc(NUM_FSENTRY, sizeof(*FS_TREE));
    if (!FS_TREE) {
        fprintf(stderr, "alloc error\n");
        return 1;
    }

    rc = _init_fil(0, -1, "camp", 0777, FS_DIR, UID_YOU, GID_EXPLORERS);
    if (rc) {
        fprintf(stderr, "init error\n");
        atl_fini();
        return 1;
    }

    rc = _gen_level(&id, 1);
    if (rc) {
        fprintf(stderr, "init error\n");
        atl_fini();
        return 1;
    }

    _init_data();

    return 0;
}

void atl_fini() {
    int i;

    for (i = 0; i < NUM_FSENTRY; ++i)
        free(FS_TREE[i].name);

    free(FS_TREE);
}