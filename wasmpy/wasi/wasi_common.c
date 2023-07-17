#include "wasi_common.h"

API_FUNC wasi_args_get(uint8_t **argv, uint8_t *argv_buf)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_args_sizes_get(wasi_size_t *argc, wasi_size_t *argv_buf_size)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_environ_get(uint8_t **environ, uint8_t *envrion_buf)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_environ_sizes_get(wasi_size_t *environc, wasi_size_t *environ_buf_size)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_clock_res_get(wasi_clockid_t id, wasi_timestamp_t *resolution)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_clock_time_get(wasi_clockid_t id, wasi_timestamp_t precision, wasi_timestamp_t *time)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_advise(wasi_fd_t fd, wasi_filesize_t offset, wasi_filesize_t len, wasi_advice_t advice)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_allocate(wasi_fd_t fd, wasi_filesize_t offset, wasi_filesize_t len)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_close(wasi_fd_t fd)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_datasync(wasi_fd_t fd)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_fdstat_get(wasi_fd_t fd, wasi_fdstat_t *stat)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_fdstat_set_flags(wasi_fd_t fd, wasi_fdflags_t flags)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_fdstat_set_rights(wasi_fd_t fd, wasi_rights_t fs_rights_base, wasi_rights_t fs_rights_inheriting)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_filestat_set_size(wasi_fd_t fd, wasi_filesize_t size)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_filestat_set_times(wasi_fd_t fd, wasi_timestamp_t atim, wasi_timestamp_t mtim, wasi_fstflags_t fst_flags)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_pread(wasi_fd_t fd, wasi_iovec_array_t iovs, wasi_filesize_t offset, wasi_size_t *nread)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_prestat_get(wasi_fd_t fd, wasi_prestat_t *buf)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_prestat_dir_name(wasi_fd_t fd, uint8_t *path, wasi_size_t path_len)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_pwrite(wasi_fd_t fd, wasi_ciovec_array_t iovs, wasi_filesize_t offset, wasi_size_t *nwritten)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_read(wasi_fd_t fd, wasi_iovec_array_t iovs, wasi_size_t *nread)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_readdir(wasi_fd_t fd, uint8_t *buf, wasi_size_t buflen, wasi_dircookie_t cookie, wasi_size_t *bufused)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_renumber(wasi_fd_t fd, wasi_fd_t to)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_seek(wasi_fd_t fd, wasi_filedelta_t offset, wasi_whence_t whence, wasi_filesize_t *newoffset)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_sync(wasi_fd_t fd)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_tell(wasi_fd_t fd, wasi_filesize_t *offset)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_fd_write(wasi_fd_t fd, wasi_ciovec_array_t iovs, wasi_size_t *nwritten)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_create_directory(wasi_fd_t fd, const char *path)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_filestat_set_times(wasi_fd_t fd, wasi_lookupflags_t flags, const char *path, wasi_timestamp_t atim, wasi_timestamp_t mtim, wasi_fstflags_t fst_flags)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_link(wasi_fd_t old_fd, wasi_lookupflags_t old_flags, const char *old_path, wasi_fd_t new_fd, const char *new_path)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_open(wasi_fd_t fd, wasi_lookupflags_t dirflags, const char *path, wasi_oflags_t oflags, wasi_rights_t fs_rights_base, wasi_rights_t fs_rights_inheriting, wasi_fdflags_t fdflags, wasi_fd_t *opened_fd)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_readlink(wasi_fd_t fd, const char *path, uint8_t *buf, wasi_size_t buf_len, wasi_size_t *bufused)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_remove_directory(wasi_fd_t fd, const char *path)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_rename(wasi_fd_t fd, const char *old_path, wasi_fd_t new_fd, const char *new_path)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_symlink(const char *old_path, wasi_fd_t fd, const char *new_path)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_path_unlink_file(wasi_fd_t fd, const char *path)
{
    return wasi_errno_nosys;
}

API_VOID wasi_proc_exit(wasi_exitcode_t rval)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_proc_raise(wasi_signal_t sig)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_sched_yield(void)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_random_get(uint8_t *buf, wasi_size_t buf_len)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_sock_recv(wasi_fd_t fd, wasi_iovec_array_t ri_data, wasi_riflags_t ri_flags, wasi_size_t *ro_datalen, wasi_roflags_t *ro_flags)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_sock_send(wasi_fd_t fd, wasi_ciovec_array_t si_data, wasi_siflags_t si_flags, wasi_size_t *so_datalen)
{
    return wasi_errno_nosys;
}

API_FUNC wasi_sock_shutdown(wasi_fd_t fd, wasi_sdflags_t how)
{
    return wasi_errno_nosys;
}
