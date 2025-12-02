#define MAX_FILES 20
#define MAX_FILENAME 32

static char files[MAX_FILES][MAX_FILENAME];
static int file_count = 3;

void init_files(void);