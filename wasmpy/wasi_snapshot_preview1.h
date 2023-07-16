#ifndef WASIP1_H
#define WASIP1_H

#include <stdint.h>

#define EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C"
{
#endif

    // typenames.witx

    typedef uint32_t wasi_size_t;
    typedef uint64_t wasi_filesize_t;
    typedef uint64_t wasi_timestamp_t;
    typedef uint32_t wasi_clockid_t;
    const wasi_clockid_t wasi_clockid_realtime = 0;
    const wasi_clockid_t wasi_clockid_monotonic = 1;
    const wasi_clockid_t wasi_clockid_process_cputime_id = 2;
    const wasi_clockid_t wasi_clockid_thread_cputime_id = 3;

    typedef uint16_t wasi_errno_t;
    const wasi_errno_t wasi_errno_success = 0;
    const wasi_errno_t wasi_errno_2big = 1;
    const wasi_errno_t wasi_errno_access = 2;
    const wasi_errno_t wasi_errno_addrinuse = 3;
    const wasi_errno_t wasi_errno_addrnotavail = 4;
    const wasi_errno_t wasi_errno_afnosupport = 5;
    const wasi_errno_t wasi_errno_again = 6;
    const wasi_errno_t wasi_errno_already = 7;
    const wasi_errno_t wasi_errno_badf = 8;
    const wasi_errno_t wasi_errno_badmsg = 9;
    const wasi_errno_t wasi_errno_busy = 10;
    const wasi_errno_t wasi_errno_canceled = 11;
    const wasi_errno_t wasi_errno_child = 12;
    const wasi_errno_t wasi_errno_connaborted = 13;
    const wasi_errno_t wasi_errno_connrefused = 14;
    const wasi_errno_t wasi_errno_connreset = 15;
    const wasi_errno_t wasi_errno_deadlk = 16;
    const wasi_errno_t wasi_errno_destaddrreq = 17;
    const wasi_errno_t wasi_errno_dom = 18;
    const wasi_errno_t wasi_errno_dquot = 19;
    const wasi_errno_t wasi_errno_exist = 20;
    const wasi_errno_t wasi_errno_fault = 21;
    const wasi_errno_t wasi_errno_fbig = 22;
    const wasi_errno_t wasi_errno_hostunreach = 23;
    const wasi_errno_t wasi_errno_idrm = 24;
    const wasi_errno_t wasi_errno_ilseq = 25;
    const wasi_errno_t wasi_errno_inprogress = 26;
    const wasi_errno_t wasi_errno_intr = 27;
    const wasi_errno_t wasi_errno_inval = 28;
    const wasi_errno_t wasi_errno_io = 29;
    const wasi_errno_t wasi_errno_isconn = 30;
    const wasi_errno_t wasi_errno_isdir = 31;
    const wasi_errno_t wasi_errno_loop = 32;
    const wasi_errno_t wasi_errno_mfile = 33;
    const wasi_errno_t wasi_errno_mlink = 34;
    const wasi_errno_t wasi_errno_msgsize = 35;
    const wasi_errno_t wasi_errno_multihop = 36;
    const wasi_errno_t wasi_errno_nametoolong = 37;
    const wasi_errno_t wasi_errno_netdown = 38;
    const wasi_errno_t wasi_errno_netreset = 39;
    const wasi_errno_t wasi_errno_netunreach = 40;
    const wasi_errno_t wasi_errno_nfile = 41;
    const wasi_errno_t wasi_errno_nobufs = 42;
    const wasi_errno_t wasi_errno_nodev = 43;
    const wasi_errno_t wasi_errno_noent = 44;
    const wasi_errno_t wasi_errno_noexec = 45;
    const wasi_errno_t wasi_errno_nolck = 46;
    const wasi_errno_t wasi_errno_nolink = 47;
    const wasi_errno_t wasi_errno_nomem = 48;
    const wasi_errno_t wasi_errno_nomsg = 49;
    const wasi_errno_t wasi_errno_noprotoopt = 50;
    const wasi_errno_t wasi_errno_nospc = 51;
    const wasi_errno_t wasi_errno_nosys = 52;
    const wasi_errno_t wasi_errno_notconn = 53;
    const wasi_errno_t wasi_errno_notdir = 54;
    const wasi_errno_t wasi_errno_notempty = 55;
    const wasi_errno_t wasi_errno_notrecoverable = 56;
    const wasi_errno_t wasi_errno_notsock = 57;
    const wasi_errno_t wasi_errno_notsup = 58;
    const wasi_errno_t wasi_errno_notty = 59;
    const wasi_errno_t wasi_errno_nxio = 60;
    const wasi_errno_t wasi_errno_overflow = 61;
    const wasi_errno_t wasi_errno_ownerdead = 62;
    const wasi_errno_t wasi_errno_perm = 63;
    const wasi_errno_t wasi_errno_pipe = 64;
    const wasi_errno_t wasi_errno_proto = 65;
    const wasi_errno_t wasi_errno_protonosupport = 66;
    const wasi_errno_t wasi_errno_prototype = 67;
    const wasi_errno_t wasi_errno_range = 68;
    const wasi_errno_t wasi_errno_rofs = 69;
    const wasi_errno_t wasi_errno_spipe = 70;
    const wasi_errno_t wasi_errno_srch = 71;
    const wasi_errno_t wasi_errno_stale = 72;
    const wasi_errno_t wasi_errno_tiemdout = 73;
    const wasi_errno_t wasi_errno_txtbsy = 74;
    const wasi_errno_t wasi_errno_xdev = 75;
    const wasi_errno_t wasi_errno_notcapable = 76;

    typedef uint64_t wasi_rights_t;
    const wasi_rights_t wasi_rights_fd_datasync = 1 << 0;
    const wasi_rights_t wasi_rights_fd_read = 1 << 1;
    const wasi_rights_t wasi_rights_fd_seek = 1 << 2;
    const wasi_rights_t wasi_rights_fd_fdstat_set_flags = 1 << 3;
    const wasi_rights_t wasi_rights_fd_sync = 1 << 4;
    const wasi_rights_t wasi_rights_fd_tell = 1 << 5;
    const wasi_rights_t wasi_rights_fd_write = 1 << 6;
    const wasi_rights_t wasi_rights_fd_advise = 1 << 7;
    const wasi_rights_t wasi_rights_fd_allocate = 1 << 8;
    const wasi_rights_t wasi_rights_path_create_directory = 1 << 9;
    const wasi_rights_t wasi_rights_path_create_file = 1 << 10;
    const wasi_rights_t wasi_rights_path_link_source = 1 << 11;
    const wasi_rights_t wasi_rights_path_link_target = 1 << 12;
    const wasi_rights_t wasi_rights_path_open = 1 << 13;
    const wasi_rights_t wasi_rights_fd_readdir = 1 << 14;
    const wasi_rights_t wasi_rights_path_readlink = 1 << 15;
    const wasi_rights_t wasi_rights_path_rename_source = 1 << 16;
    const wasi_rights_t wasi_rights_path_rename_target = 1 << 17;
    const wasi_rights_t wasi_rights_path_filestat_get = 1 << 18;
    const wasi_rights_t wasi_rights_path_filestat_set_size = 1 << 19;
    const wasi_rights_t wasi_rights_path_filestat_set_times = 1 << 20;
    const wasi_rights_t wasi_rights_fd_filestat_get = 1 << 21;
    const wasi_rights_t wasi_rights_fd_filestat_set_size = 1 << 22;
    const wasi_rights_t wasi_rights_fd_filestat_set_times = 1 << 23;
    const wasi_rights_t wasi_rights_path_symlink = 1 << 24;
    const wasi_rights_t wasi_rights_path_remove_directory = 1 << 25;
    const wasi_rights_t wasi_rights_path_unlink_file = 1 << 26;
    const wasi_rights_t wasi_rights_poll_fd_readwrite = 1 << 27;
    const wasi_rights_t wasi_rights_sock_shutdown = 1 << 28;
    const wasi_rights_t wasi_rights_sock_accept = 1 << 29;

    typedef uint32_t wasi_fd_t;

    typedef struct
    {
        uint8_t *buf;
        wasi_size_t buf_len;
    } wasi_iovec_t;

    typedef struct
    {
        const uint8_t *buf;
        wasi_size_t buf_len;
    } wasi_ciovec_t;

    typedef wasi_iovec_t *wasi_iovec_array_t;
    typedef wasi_ciovec_t *wasi_ciovec_array_t;
    typedef int64_t wasi_filedelta_t;
    typedef uint8_t wasi_whence_t;
    const wasi_whence_t wasi_whence_set = 0;
    const wasi_whence_t wasi_whence_cur = 1;
    const wasi_whence_t wasi_whence_end = 2;

    typedef uint64_t wasi_dircookie_t;
    typedef uint32_t wasi_dirnamlen_t;
    typedef uint64_t wasi_inode_t;
    typedef uint8_t wasi_filetype_t;
    const wasi_filetype_t wasi_filetype_unknown = 0;
    const wasi_filetype_t wasi_filetype_block_device = 1;
    const wasi_filetype_t wasi_filetype_character_device = 2;
    const wasi_filetype_t wasi_filetype_directory = 3;
    const wasi_filetype_t wasi_filetype_regular_file = 4;
    const wasi_filetype_t wasi_filetype_socket_dgram = 5;
    const wasi_filetype_t wasi_filetype_socket_stream = 6;
    const wasi_filetype_t wasi_filetype_symbolic_link = 7;

    typedef struct
    {
        wasi_dircookie_t d_next;
        wasi_inode_t d_ino;
        wasi_dirnamlen_t d_namlen;
        wasi_filetype_t d_type;
    } wasi_dirent_t;

    typedef uint8_t wasi_advice_t;
    const wasi_advice_t wasi_advice_normal = 0;
    const wasi_advice_t wasi_advice_sequential = 1;
    const wasi_advice_t wasi_advice_random = 2;
    const wasi_advice_t wasi_advice_willneed = 3;
    const wasi_advice_t wasi_advice_dontneed = 4;
    const wasi_advice_t wasi_advice_noreuse = 5;

    typedef uint16_t wasi_fdflags_t;
    const wasi_fdflags_t wasi_fdflags_append = 1 << 0;
    const wasi_fdflags_t wasi_fdflags_dsync = 1 << 1;
    const wasi_fdflags_t wasi_fdflags_nonblock = 1 << 2;
    const wasi_fdflags_t wasi_fdflags_rsync = 1 << 3;
    const wasi_fdflags_t wasi_fdflags_sync = 1 << 4;

    typedef struct
    {
        wasi_filetype_t fs_filetype;
        wasi_fdflags_t fs_flags;
        wasi_rights_t fs_rights_base;
        wasi_rights_t fs_rights_inheriting;
    } wasi_fdstat_t;

    typedef uint64_t wasi_device_t;
    typedef uint16_t wasi_fstflags_t;
    const wasi_fstflags_t wasi_fstflags_atim = 1 << 0;
    const wasi_fstflags_t wasi_fstflags_atim_now = 1 << 1;
    const wasi_fstflags_t wasi_fstflags_mtim = 1 << 2;
    const wasi_fstflags_t wasi_fstflags_mtim_now = 1 << 3;

    typedef uint32_t wasi_lookupflags_t;
    const wasi_lookupflags_t wasi_lookupflags_symlink_follow = 1 << 0;

    typedef uint16_t wasi_oflags_t;
    const wasi_oflags_t wasi_oflags_creat = 1 << 0;
    const wasi_oflags_t wasi_oflags_directory = 1 << 1;
    const wasi_oflags_t wasi_oflags_excl = 1 << 2;
    const wasi_oflags_t wasi_oflags_trunc = 1 << 3;

    typedef uint64_t wasi_linkcount_t;

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

    typedef uint64_t wasi_userdata_t;
    typedef uint8_t wasi_eventtype_t;
    const wasi_eventtype_t wasi_eventtype_clock = 0;
    const wasi_eventtype_t wasi_eventtype_fd_read = 1;
    const wasi_eventtype_t wasi_eventtype_fd_write = 2;

    typedef uint16_t wasi_eventrwflags_t;
    const wasi_eventrwflags_t wasi_eventrwflags_fd_readwrite_hangup = 1 << 0;

    typedef struct
    {
        wasi_filesize_t nbytes;
        wasi_eventrwflags_t flags;
    } wasi_event_fd_readwrite_t;

    typedef struct
    {
        wasi_userdata_t userdata;
        wasi_errno_t error;
        wasi_eventtype_t type;
        wasi_event_fd_readwrite_t fd_readwrite;
    } wasi_event_t;

    typedef uint16_t wasi_subclockflags_t;
    const wasi_subclockflags_t wasi_subclockflags_subscription_clock_abstime = 1 << 0;

    typedef struct
    {
        wasi_clockid_t id;
        wasi_timestamp_t timeout;
        wasi_timestamp_t precision;
        wasi_subclockflags_t flags;
    } wasi_subscription_clock_t;

    typedef struct
    {
        wasi_fd_t file_descriptor;
    } wasi_subscription_fd_readwrite_t;

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

    typedef uint32_t wasi_exitcode_t;
    typedef uint8_t wasi_signal_t;
    const wasi_signal_t wasi_signal_none = 0;
    const wasi_signal_t wasi_signal_hup = 1;
    const wasi_signal_t wasi_signal_int = 2;
    const wasi_signal_t wasi_signal_quit = 3;
    const wasi_signal_t wasi_signal_ill = 4;
    const wasi_signal_t wasi_signal_trap = 5;
    const wasi_signal_t wasi_signal_abrt = 6;
    const wasi_signal_t wasi_signal_bus = 7;
    const wasi_signal_t wasi_signal_fpe = 8;
    const wasi_signal_t wasi_signal_kill = 9;
    const wasi_signal_t wasi_signal_usr1 = 10;
    const wasi_signal_t wasi_signal_segv = 11;
    const wasi_signal_t wasi_signal_usr2 = 12;
    const wasi_signal_t wasi_signal_pipe = 13;
    const wasi_signal_t wasi_signal_alrm = 14;
    const wasi_signal_t wasi_signal_term = 15;
    const wasi_signal_t wasi_signal_chld = 16;
    const wasi_signal_t wasi_signal_cont = 17;
    const wasi_signal_t wasi_signal_stop = 18;
    const wasi_signal_t wasi_signal_tstp = 19;
    const wasi_signal_t wasi_signal_ttin = 20;
    const wasi_signal_t wasi_signal_ttou = 21;
    const wasi_signal_t wasi_signal_urg = 22;
    const wasi_signal_t wasi_signal_xcpu = 23;
    const wasi_signal_t wasi_signal_xfsz = 24;
    const wasi_signal_t wasi_signal_vtalrm = 25;
    const wasi_signal_t wasi_signal_prof = 26;
    const wasi_signal_t wasi_signal_winch = 27;
    const wasi_signal_t wasi_signal_poll = 28;
    const wasi_signal_t wasi_signal_pwr = 29;
    const wasi_signal_t wasi_signal_sys = 30;

    typedef uint16_t wasi_riflags_t;
    const wasi_riflags_t wasi_riflags_recv_peek = 1 << 0;
    const wasi_riflags_t wasi_riflags_recv_waitall = 1 << 1;

    typedef uint16_t wasi_roflags_t;
    const wasi_roflags_t wasi_roflags_recv_data_truncated = 1 << 0;

    typedef uint16_t wasi_siflags_t;
    typedef uint8_t wasi_sdflags_t;
    const wasi_sdflags_t wasi_sdflags_rd = 1 << 0;
    const wasi_sdflags_t wasi_sdflags_wr = 1 << 1;

    typedef uint8_t wasi_preopentype_t;
    const wasi_preopentype_t wasi_preopentype_dir = 0;

    typedef struct
    {
        wasi_size_t pr_name_len;
    } wasi_prestat_dir_t;

    typedef struct
    {
        wasi_preopentype_t tag;
        union
        {
            wasi_prestat_dir_t dir;
        } u;
    } wasi_prestat_t;

    // wasi_snapshot_preview1.witx

#define API_FUNC EXPORT wasi_errno_t
#define API_VOID EXPORT void

    API_FUNC wasi_args_get(uint8_t **argv, uint8_t *argv_buf);
    API_FUNC wasi_args_sizes_get(wasi_size_t *argc, wasi_size_t *argv_buf_size);
    API_FUNC wasi_environ_get(uint8_t **environ, uint8_t *envrion_buf);
    API_FUNC wasi_environ_sizes_get(wasi_size_t *environc, wasi_size_t *environ_buf_size);
    API_FUNC wasi_clock_res_get(wasi_clockid_t id, wasi_timestamp_t *resolution);
    API_FUNC wasi_clock_time_get(wasi_clockid_t id, wasi_timestamp_t precision, wasi_timestamp_t *time);
    API_FUNC wasi_fd_advise(wasi_fd_t fd, wasi_filesize_t offset, wasi_filesize_t len, wasi_advice_t advice);
    API_FUNC wasi_fd_allocate(wasi_fd_t fd, wasi_filesize_t offset, wasi_filesize_t len);
    API_FUNC wasi_fd_close(wasi_fd_t fd);
    API_FUNC wasi_fd_datasync(wasi_fd_t fd);
    API_FUNC wasi_fd_fdstat_get(wasi_fd_t fd, wasi_fdstat_t *stat);
    API_FUNC wasi_fd_fdstat_set_flags(wasi_fd_t fd, wasi_fdflags_t flags);
    API_FUNC wasi_fd_fdstat_set_rights(wasi_fd_t fd, wasi_rights_t fs_rights_base, wasi_rights_t fs_rights_inheriting);
    API_FUNC wasi_fd_filestat_get(wasi_fd_t fd, wasi_filestat_t *buf);
    API_FUNC wasi_fd_filestat_set_size(wasi_fd_t fd, wasi_filesize_t size);
    API_FUNC wasi_fd_filestat_set_times(wasi_fd_t fd, wasi_timestamp_t atim, wasi_timestamp_t mtim, wasi_fstflags_t fst_flags);
    API_FUNC wasi_fd_pread(wasi_fd_t fd, wasi_iovec_array_t iovs, wasi_filesize_t offset, wasi_size_t *nread);
    API_FUNC wasi_fd_prestat_get(wasi_fd_t fd, wasi_prestat_t *buf);
    API_FUNC wasi_fd_prestat_dir_name(wasi_fd_t fd, uint8_t *path, wasi_size_t path_len);
    API_FUNC wasi_fd_pwrite(wasi_fd_t fd, wasi_ciovec_array_t iovs, wasi_filesize_t offset, wasi_size_t *nwritten);
    API_FUNC wasi_fd_read(wasi_fd_t fd, wasi_iovec_array_t iovs, wasi_size_t *nread);
    API_FUNC wasi_fd_readdir(wasi_fd_t fd, uint8_t *buf, wasi_size_t buflen, wasi_dircookie_t cookie, wasi_size_t *bufused);
    API_FUNC wasi_fd_renumber(wasi_fd_t fd, wasi_fd_t to);
    API_FUNC wasi_fd_seek(wasi_fd_t fd, wasi_filedelta_t offset, wasi_whence_t whence, wasi_filesize_t *newoffset);
    API_FUNC wasi_fd_sync(wasi_fd_t fd);
    API_FUNC wasi_fd_tell(wasi_fd_t fd, wasi_filesize_t *offset);
    API_FUNC wasi_fd_write(wasi_fd_t fd, wasi_ciovec_array_t iovs, wasi_size_t *nwritten);
    API_FUNC wasi_path_create_directory(wasi_fd_t fd, const char *path);
    API_FUNC wasi_path_filestat_get(wasi_fd_t fd, wasi_lookupflags_t flags, const char *path, wasi_filestat_t *buf);
    API_FUNC wasi_path_filestat_set_times(wasi_fd_t fd, wasi_lookupflags_t flags, const char *path, wasi_timestamp_t atim, wasi_timestamp_t mtim, wasi_fstflags_t fst_flags);
    API_FUNC wasi_path_link(wasi_fd_t old_fd, wasi_lookupflags_t old_flags, const char *old_path, wasi_fd_t new_fd, const char *new_path);
    API_FUNC wasi_path_open(wasi_fd_t fd, wasi_lookupflags_t dirflags, const char *path, wasi_oflags_t oflags, wasi_rights_t fs_rights_base, wasi_rights_t fs_rights_inheriting, wasi_fdflags_t fdflags, wasi_fd_t *opened_fd);
    API_FUNC wasi_path_readlink(wasi_fd_t fd, const char *path, uint8_t *buf, wasi_size_t buf_len, wasi_size_t *bufused);
    API_FUNC wasi_path_remove_directory(wasi_fd_t fd, const char *path);
    API_FUNC wasi_path_rename(wasi_fd_t fd, const char *old_path, wasi_fd_t new_fd, const char *new_path);
    API_FUNC wasi_path_symlink(const char *old_path, wasi_fd_t fd, const char *new_path);
    API_FUNC wasi_path_unlink_file(wasi_fd_t fd, const char *path);
    API_FUNC wasi_poll_oneoff(const wasi_subscription_t *in, wasi_event_t *out, wasi_size_t nsubscriptions, wasi_size_t *nevents);
    API_VOID wasi_proc_exit(wasi_exitcode_t rval);
    API_FUNC wasi_proc_raise(wasi_signal_t sig);
    API_FUNC wasi_sched_yield(void);
    API_FUNC wasi_random_get(uint8_t *buf, wasi_size_t buf_len);
    API_FUNC wasi_sock_accept(wasi_fd_t fd, wasi_fdflags_t flags, wasi_fd_t *opened_sock);
    API_FUNC wasi_sock_recv(wasi_fd_t fd, wasi_iovec_array_t ri_data, wasi_riflags_t ri_flags, wasi_size_t *ro_datalen, wasi_roflags_t *ro_flags);
    API_FUNC wasi_sock_send(wasi_fd_t fd, wasi_ciovec_array_t si_data, wasi_siflags_t si_flags, wasi_size_t *so_datalen);
    API_FUNC wasi_sock_shutdown(wasi_fd_t fd, wasi_sdflags_t how);

#ifdef __cplusplus
}
#endif

#endif
