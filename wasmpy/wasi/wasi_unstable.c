#include "wasi_common.h"

const wasi_whence_t wasi_whence_cur = 0;
const wasi_whence_t wasi_whence_end = 1;
const wasi_whence_t wasi_whence_set = 2;

typedef uint32_t wasi_linkcount_t;

typedef struct
{
    wasi_device_t dev;
    wasi_inode_t ino;
    wasi_filetype_t filetype;
    wasi_linkcount_t nlink;
    wasi_filesize_t size;
    wasi_timestamp_t atim;
    wasi_timestamp_t mtim;
    wasi_timestamp_t ctim;
} wasi_filestat_t;

typedef struct
{
    wasi_userdata_t identifier;
    wasi_clockid_t id;
    wasi_timestamp_t timeout;
    wasi_timestamp_t precision;
    wasi_subclockflags_t flags;
} wasi_subscription_clock_t;

typedef struct
{
    wasi_eventtype_t tag;
    union
    {
        wasi_subscription_clock_t clock;
        wasi_subscription_fd_readwrite_t fd_read;
        wasi_subscription_fd_readwrite_t fd_write;
    } u;
} wasi_subscription_u_t;

typedef struct
{
    wasi_userdata_t userdata;
    wasi_subscription_u_t u;
} wasi_subscription_t;

API wasi_errno_t wasi_fd_filestat_get(wasi_fd_t fd, wasi_filestat_t *buf)
{
    return wasi_errno_nosys;
}

API wasi_errno_t wasi_path_filestat_get(wasi_fd_t fd, wasi_lookupflags_t flags, const char *path, wasi_filestat_t *buf)
{
    return wasi_errno_nosys;
}

API wasi_errno_t wasi_poll_oneoff(const wasi_subscription_t *in, wasi_event_t *out, wasi_size_t nsubscriptions, wasi_size_t *nevents)
{
    return wasi_errno_nosys;
}
