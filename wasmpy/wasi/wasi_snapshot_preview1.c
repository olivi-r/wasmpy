#include "wasi_snapshot_preview1.h"

API_FUNC wasi_fd_filestat_get(wasi_fd_t fd, wasi_filestat_t *buf)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_filestat_get(wasi_fd_t fd, wasi_lookupflags_t flags, const char *path, wasi_filestat_t *buf)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_poll_oneoff(const wasi_subscription_t *in, wasi_event_t *out, wasi_size_t nsubscriptions, wasi_size_t *nevents)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_sock_accept(wasi_fd_t fd, wasi_fdflags_t flags, wasi_fd_t *opened_sock)
{
    return wasi_errno_nosys;
}
