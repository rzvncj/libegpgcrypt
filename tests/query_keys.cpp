#include <context.hpp>
#include <ctime>
#include <data_buffers.hpp>
#include <getopt.h>
#include <iostream>
#include <keys.hpp>

using namespace egpgcrypt;
using namespace std;

void print_help(const string& program_name)
{
    cout << "Usage: " << program_name << " options\n"
         << "  -h  --help            Display this usage information.\n"
         << "  -s  --secret pattern  Secret key to look for.\n"
         << "  -p  --public pattern  Public key to look for.\n"
         << flush;
}

void print_key(const key& k)
{
    if (!k) {
        cerr << "No key found.\n";
        return;
    }

    long       expires  = k.expires();
    struct tm* timeinfo = localtime(&expires);

    cout << "Key found for address <" << k.email() << ">.\nrevoked: " << boolalpha << k.revoked()
         << ", expired: " << k.expired() << ", invalid: " << k.invalid() << ", disabled: " << k.disabled()
         << ", secret: " << k.secret() << ", can_encrypt: " << k.can_encrypt() << "\nexpires: " << asctime(timeinfo)
         << endl;
}

int main(int argc, char** argv)
{
    try {

        const char* const   short_options  = "hs:p:";
        const struct option long_options[] = {{"help", 0, nullptr, 'h'},
                                              {"secret", 1, nullptr, 's'},
                                              {"public", 1, nullptr, 'p'},
                                              {nullptr, 0, nullptr, 0}};

        string secret_pattern, public_pattern;
        int    next_option;

        do {

            next_option = getopt_long(argc, argv, short_options, long_options, nullptr);
            switch (next_option) {

            case 's':
                secret_pattern = optarg;
                break;

            case 'p':
                public_pattern = optarg;
                break;

            case -1: // no more arguments
                break;

            case 'h':
            case '?':
            default:
                print_help(argv[0]);
                return 0;
            }

        } while (next_option != -1);

        if (secret_pattern.empty() && public_pattern.empty()) {
            print_help(argv[0]);
            return 0;
        }

        // Begin actual example

        context ctx;

        if (!secret_pattern.empty()) {
            key k = ctx.find_key(secret_pattern, true, false);
            print_key(k);
        }

        if (!public_pattern.empty()) {
            key k = ctx.find_key(public_pattern);
            print_key(k);
        }

    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}
