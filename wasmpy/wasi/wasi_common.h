#ifndef WASI_COMMON_H
#define WASI_COMMON_H
#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API __attribute__((visibility("default")))
#endif
#include <stdint.h>

typedef uint32_t wasi_size_t;
typedef uint64_t wasi_filesize_t;
typedef uint64_t wasi_timestamp_t;

typedef uint32_t wasi_clockid_t;
extern const wasi_clockid_t wasi_clockid_realtime;
extern const wasi_clockid_t wasi_clockid_monotonic;
extern const wasi_clockid_t wasi_clockid_process_cputime_id;
extern const wasi_clockid_t wasi_clockid_thread_cputime_id;

typedef uint16_t wasi_errno_t;
extern const wasi_errno_t wasi_errno_success;
extern const wasi_errno_t wasi_errno_2big;
extern const wasi_errno_t wasi_errno_access;
extern const wasi_errno_t wasi_errno_addrinuse;
extern const wasi_errno_t wasi_errno_addrnotavail;
extern const wasi_errno_t wasi_errno_afnosupport;
extern const wasi_errno_t wasi_errno_again;
extern const wasi_errno_t wasi_errno_already;
extern const wasi_errno_t wasi_errno_badf;
extern const wasi_errno_t wasi_errno_badmsg;
extern const wasi_errno_t wasi_errno_busy;
extern const wasi_errno_t wasi_errno_canceled;
extern const wasi_errno_t wasi_errno_child;
extern const wasi_errno_t wasi_errno_connaborted;
extern const wasi_errno_t wasi_errno_connrefused;
extern const wasi_errno_t wasi_errno_connreset;
extern const wasi_errno_t wasi_errno_deadlk;
extern const wasi_errno_t wasi_errno_destaddrreq;
extern const wasi_errno_t wasi_errno_dom;
extern const wasi_errno_t wasi_errno_dquot;
extern const wasi_errno_t wasi_errno_exist;
extern const wasi_errno_t wasi_errno_fault;
extern const wasi_errno_t wasi_errno_fbig;
extern const wasi_errno_t wasi_errno_hostunreach;
extern const wasi_errno_t wasi_errno_idrm;
extern const wasi_errno_t wasi_errno_ilseq;
extern const wasi_errno_t wasi_errno_inprogress;
extern const wasi_errno_t wasi_errno_intr;
extern const wasi_errno_t wasi_errno_inval;
extern const wasi_errno_t wasi_errno_io;
extern const wasi_errno_t wasi_errno_isconn;
extern const wasi_errno_t wasi_errno_isdir;
extern const wasi_errno_t wasi_errno_loop;
extern const wasi_errno_t wasi_errno_mfile;
extern const wasi_errno_t wasi_errno_mlink;
extern const wasi_errno_t wasi_errno_msgsize;
extern const wasi_errno_t wasi_errno_multihop;
extern const wasi_errno_t wasi_errno_nametoolong;
extern const wasi_errno_t wasi_errno_netdown;
extern const wasi_errno_t wasi_errno_netreset;
extern const wasi_errno_t wasi_errno_netunreach;
extern const wasi_errno_t wasi_errno_nfile;
extern const wasi_errno_t wasi_errno_nobufs;
extern const wasi_errno_t wasi_errno_nodev;
extern const wasi_errno_t wasi_errno_noent;
extern const wasi_errno_t wasi_errno_noexec;
extern const wasi_errno_t wasi_errno_nolck;
extern const wasi_errno_t wasi_errno_nolink;
extern const wasi_errno_t wasi_errno_nomem;
extern const wasi_errno_t wasi_errno_nomsg;
extern const wasi_errno_t wasi_errno_noprotoopt;
extern const wasi_errno_t wasi_errno_nospc;
extern const wasi_errno_t wasi_errno_nosys;
extern const wasi_errno_t wasi_errno_notconn;
extern const wasi_errno_t wasi_errno_notdir;
extern const wasi_errno_t wasi_errno_notempty;
extern const wasi_errno_t wasi_errno_notrecoverable;
extern const wasi_errno_t wasi_errno_notsock;
extern const wasi_errno_t wasi_errno_notsup;
extern const wasi_errno_t wasi_errno_notty;
extern const wasi_errno_t wasi_errno_nxio;
extern const wasi_errno_t wasi_errno_overflow;
extern const wasi_errno_t wasi_errno_ownerdead;
extern const wasi_errno_t wasi_errno_perm;
extern const wasi_errno_t wasi_errno_pipe;
extern const wasi_errno_t wasi_errno_proto;
extern const wasi_errno_t wasi_errno_protonosupport;
extern const wasi_errno_t wasi_errno_prototype;
extern const wasi_errno_t wasi_errno_range;
extern const wasi_errno_t wasi_errno_rofs;
extern const wasi_errno_t wasi_errno_spipe;
extern const wasi_errno_t wasi_errno_srch;
extern const wasi_errno_t wasi_errno_stale;
extern const wasi_errno_t wasi_errno_tiemdout;
extern const wasi_errno_t wasi_errno_txtbsy;
extern const wasi_errno_t wasi_errno_xdev;
extern const wasi_errno_t wasi_errno_notcapable;

typedef uint64_t wasi_rights_t;
extern const wasi_rights_t wasi_rights_fd_datasync;
extern const wasi_rights_t wasi_rights_fd_read;
extern const wasi_rights_t wasi_rights_fd_seek;
extern const wasi_rights_t wasi_rights_fd_fdstat_set_flags;
extern const wasi_rights_t wasi_rights_fd_sync;
extern const wasi_rights_t wasi_rights_fd_tell;
extern const wasi_rights_t wasi_rights_fd_write;
extern const wasi_rights_t wasi_rights_fd_advise;
extern const wasi_rights_t wasi_rights_fd_allocate;
extern const wasi_rights_t wasi_rights_path_create_directory;
extern const wasi_rights_t wasi_rights_path_create_file;
extern const wasi_rights_t wasi_rights_path_link_source;
extern const wasi_rights_t wasi_rights_path_link_target;
extern const wasi_rights_t wasi_rights_path_open;
extern const wasi_rights_t wasi_rights_fd_readdir;
extern const wasi_rights_t wasi_rights_path_readlink;
extern const wasi_rights_t wasi_rights_path_rename_source;
extern const wasi_rights_t wasi_rights_path_rename_target;
extern const wasi_rights_t wasi_rights_path_filestat_get;
extern const wasi_rights_t wasi_rights_path_filestat_set_size;
extern const wasi_rights_t wasi_rights_path_filestat_set_times;
extern const wasi_rights_t wasi_rights_fd_filestat_get;
extern const wasi_rights_t wasi_rights_fd_filestat_set_size;
extern const wasi_rights_t wasi_rights_fd_filestat_set_times;
extern const wasi_rights_t wasi_rights_path_symlink;
extern const wasi_rights_t wasi_rights_path_remove_directory;
extern const wasi_rights_t wasi_rights_path_unlink_file;
extern const wasi_rights_t wasi_rights_poll_fd_readwrite;
extern const wasi_rights_t wasi_rights_sock_shutdown;

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
typedef uint64_t wasi_dircookie_t;
typedef uint32_t wasi_dirnamlen_t;
typedef uint64_t wasi_inode_t;

typedef uint8_t wasi_filetype_t;
extern const wasi_filetype_t wasi_filetype_unknown;
extern const wasi_filetype_t wasi_filetype_block_device;
extern const wasi_filetype_t wasi_filetype_character_device;
extern const wasi_filetype_t wasi_filetype_directory;
extern const wasi_filetype_t wasi_filetype_regular_file;
extern const wasi_filetype_t wasi_filetype_socket_dgram;
extern const wasi_filetype_t wasi_filetype_socket_stream;
extern const wasi_filetype_t wasi_filetype_symbolic_link;

typedef struct
{
    wasi_dircookie_t d_next;
    wasi_inode_t d_ino;
    wasi_dirnamlen_t d_namlen;
    wasi_filetype_t d_type;
} wasi_dirent_t;

typedef uint8_t wasi_advice_t;
extern const wasi_advice_t wasi_advice_normal;
extern const wasi_advice_t wasi_advice_sequential;
extern const wasi_advice_t wasi_advice_random;
extern const wasi_advice_t wasi_advice_willneed;
extern const wasi_advice_t wasi_advice_dontneed;
extern const wasi_advice_t wasi_advice_noreuse;

typedef uint16_t wasi_fdflags_t;
extern const wasi_fdflags_t wasi_fdflags_append;
extern const wasi_fdflags_t wasi_fdflags_dsync;
extern const wasi_fdflags_t wasi_fdflags_nonblock;
extern const wasi_fdflags_t wasi_fdflags_rsync;
extern const wasi_fdflags_t wasi_fdflags_sync;

typedef struct
{
    wasi_filetype_t fs_filetype;
    wasi_fdflags_t fs_flags;
    wasi_rights_t fs_rights_base;
    wasi_rights_t fs_rights_inheriting;
} wasi_fdstat_t;

typedef uint64_t wasi_device_t;
typedef uint16_t wasi_fstflags_t;
extern const wasi_fstflags_t wasi_fstflags_atim;
extern const wasi_fstflags_t wasi_fstflags_atim_now;
extern const wasi_fstflags_t wasi_fstflags_mtim;
extern const wasi_fstflags_t wasi_fstflags_mtim_now;

typedef uint32_t wasi_lookupflags_t;
extern const wasi_lookupflags_t wasi_lookupflags_symlink_follow;

typedef uint16_t wasi_oflags_t;
extern const wasi_oflags_t wasi_oflags_creat;
extern const wasi_oflags_t wasi_oflags_directory;
extern const wasi_oflags_t wasi_oflags_excl;
extern const wasi_oflags_t wasi_oflags_trunc;

typedef uint64_t wasi_userdata_t;
typedef uint8_t wasi_eventtype_t;
extern const wasi_eventtype_t wasi_eventtype_clock;
extern const wasi_eventtype_t wasi_eventtype_fd_read;
extern const wasi_eventtype_t wasi_eventtype_fd_write;

typedef uint16_t wasi_eventrwflags_t;
extern const wasi_eventrwflags_t wasi_eventrwflags_fd_readwrite_hangup;

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
extern const wasi_subclockflags_t wasi_subclockflags_subscription_clock_abstime;

typedef struct
{
    wasi_fd_t file_descriptor;
} wasi_subscription_fd_readwrite_t;

typedef uint32_t wasi_exitcode_t;

typedef uint8_t wasi_signal_t;
extern const wasi_signal_t wasi_signal_none;
extern const wasi_signal_t wasi_signal_hup;
extern const wasi_signal_t wasi_signal_int;
extern const wasi_signal_t wasi_signal_quit;
extern const wasi_signal_t wasi_signal_ill;
extern const wasi_signal_t wasi_signal_trap;
extern const wasi_signal_t wasi_signal_abrt;
extern const wasi_signal_t wasi_signal_bus;
extern const wasi_signal_t wasi_signal_fpe;
extern const wasi_signal_t wasi_signal_kill;
extern const wasi_signal_t wasi_signal_usr1;
extern const wasi_signal_t wasi_signal_segv;
extern const wasi_signal_t wasi_signal_usr2;
extern const wasi_signal_t wasi_signal_pipe;
extern const wasi_signal_t wasi_signal_alrm;
extern const wasi_signal_t wasi_signal_term;
extern const wasi_signal_t wasi_signal_chld;
extern const wasi_signal_t wasi_signal_cont;
extern const wasi_signal_t wasi_signal_stop;
extern const wasi_signal_t wasi_signal_tstp;
extern const wasi_signal_t wasi_signal_ttin;
extern const wasi_signal_t wasi_signal_ttou;
extern const wasi_signal_t wasi_signal_urg;
extern const wasi_signal_t wasi_signal_xcpu;
extern const wasi_signal_t wasi_signal_xfsz;
extern const wasi_signal_t wasi_signal_vtalrm;
extern const wasi_signal_t wasi_signal_prof;
extern const wasi_signal_t wasi_signal_winch;
extern const wasi_signal_t wasi_signal_poll;
extern const wasi_signal_t wasi_signal_pwr;
extern const wasi_signal_t wasi_signal_sys;

typedef uint16_t wasi_riflags_t;
extern const wasi_riflags_t wasi_riflags_recv_peek;
extern const wasi_riflags_t wasi_riflags_recv_waitall;

typedef uint16_t wasi_roflags_t;
extern const wasi_roflags_t wasi_roflags_recv_data_truncated;

typedef uint16_t wasi_siflags_t;

typedef uint8_t wasi_sdflags_t;
extern const wasi_sdflags_t wasi_sdflags_rd;
extern const wasi_sdflags_t wasi_sdflags_wr;

typedef uint8_t wasi_preopentype_t;
extern const wasi_preopentype_t wasi_preopentype_dir;

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

#endif
