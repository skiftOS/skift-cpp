#include <libsystem/Stdio.h>

using namespace libsystem;

extern "C" int main(int argc, char const *argv[])
{
    __unused(argc), __unused(argv);

    print("Hello, world!");

    return 0;
}
