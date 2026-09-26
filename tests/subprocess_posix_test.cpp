#include "../src/utils/subprocess_posix.hpp"
#include <cassert>
int main() {
    auto args = subprocess::splitCommand("tool --name \"two words\" 'three words' escaped\\ value");
    assert((args == std::vector<std::string>{"tool", "--name", "two words", "three words", "escaped value"}));
    assert(subprocess::splitCommand("tool 'unterminated").empty());
    subprocess::Popen child("/usr/bin/wc -c");
    assert(child.valid());
    assert(child.m_stdin.write("macOS", 5));
    assert(child.close() == 0);
}
