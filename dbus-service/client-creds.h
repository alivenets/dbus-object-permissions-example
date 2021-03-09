#pragma once

#include <sys/types.h>

struct ClientCredentials
{
    pid_t pid;
    uid_t uid;
    gid_t gid;
    std::vector<gid_t> supplementaryGids;
    std::string linuxSecurityLabel;
};