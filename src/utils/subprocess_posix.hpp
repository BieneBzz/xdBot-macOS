#pragma once
// No shell is involved: paths and renderer arguments are passed directly to FFmpeg.
#include <cerrno>
#include <cctype>
#include <fcntl.h>
#include <spawn.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <crt_externs.h>

namespace subprocess {
inline std::vector<std::string> splitCommand(const std::string& command) {
    std::vector<std::string> args;
    std::string arg;
    char quote = 0;
    bool escaped = false, started = false;
    for (char c : command) {
        if (escaped) { arg += c; escaped = false; started = true; }
        else if (c == '\\' && quote != '\'') { escaped = true; started = true; }
        else if (quote) { if (c == quote) quote = 0; else arg += c; }
        else if (c == '"' || c == '\'') { quote = c; started = true; }
        else if (std::isspace(static_cast<unsigned char>(c))) {
            if (started) { args.push_back(arg); arg.clear(); started = false; }
        } else { arg += c; started = true; }
    }
    if (quote || escaped) return {};
    if (started) args.push_back(arg);
    return args;
}
// Escape a value embedded inside a quoted command argument (not shell quoting).
inline std::string escapeQuoted(const std::string& value) {
    std::string result;
    for (char c : value) {
        if (c == '"' || c == '\\') result += '\\';
        result += c;
    }
    return result;
}
struct PipePair {
    int fd = -1;
    bool failed = false;
    bool write(const void* data, size_t size) {
        auto bytes = static_cast<const char*>(data);
        while (size) {
            ssize_t n = ::write(fd, bytes, size);
            if (n < 0 && errno == EINTR) continue;
            if (n <= 0) { failed = true; return false; }
            bytes += n; size -= static_cast<size_t>(n);
        }
        return true;
    }
    void close() { if (fd >= 0) ::close(std::exchange(fd, -1)); }
};
class Popen {
    pid_t pid = -1;
    int result = 0;
public:
    PipePair m_stdin;
    Popen() = default;
    Popen(const Popen&) = delete;
    Popen& operator=(const Popen&) = delete;
    Popen(Popen&& other) noexcept { *this = std::move(other); }
    Popen& operator=(Popen&& other) noexcept {
        if (this != &other) {
            close();
            pid = std::exchange(other.pid, -1);
            result = other.result;
            m_stdin.fd = std::exchange(other.m_stdin.fd, -1);
            m_stdin.failed = other.m_stdin.failed;
        }
        return *this;
    }
    explicit Popen(const std::string& command) {
        auto args = splitCommand(command);
        if (args.empty()) { result = EINVAL; return; }
        std::vector<char*> argv;
        for (auto& arg : args) argv.push_back(arg.data());
        argv.push_back(nullptr);
        int fds[2];
        if (::pipe(fds)) { result = errno; return; }
        // A failed encoder must not terminate the game with SIGPIPE.
        ::fcntl(fds[1], F_SETNOSIGPIPE, 1);
        ::fcntl(fds[0], F_SETFD, FD_CLOEXEC);
        ::fcntl(fds[1], F_SETFD, FD_CLOEXEC);
        posix_spawn_file_actions_t actions;
        posix_spawn_file_actions_init(&actions);
        posix_spawn_file_actions_adddup2(&actions, fds[0], STDIN_FILENO);
        posix_spawn_file_actions_addclose(&actions, fds[0]);
        posix_spawn_file_actions_addclose(&actions, fds[1]);
        result = posix_spawnp(&pid, argv[0], &actions, nullptr, argv.data(), *_NSGetEnviron());
        posix_spawn_file_actions_destroy(&actions);
        ::close(fds[0]);
        if (result) { ::close(fds[1]); pid = -1; }
        else m_stdin.fd = fds[1];
    }
    ~Popen() { close(); }
    bool valid() const { return pid > 0; }
    int wait() {
        if (pid < 0) return result;
        int status = 0;
        pid_t waited;
        do { waited = ::waitpid(pid, &status, 0); } while (waited < 0 && errno == EINTR);
        result = waited < 0 ? errno : WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
        pid = -1;
        return result;
    }
    int close(bool should_wait = true) {
        m_stdin.close();
        if (should_wait) wait();
        return result ? result : (m_stdin.failed ? EPIPE : 0);
    }
};
}
