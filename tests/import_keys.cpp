#include <crypto.hpp>
#include <data_buffers.hpp>
#include <getopt.h>
#include <iostream>


using namespace egpgcrypt;
using namespace std;


void print_help(const string& program_name)
{
    cout << "Usage: " << program_name << " options\n"
         << "  -h  --help            Display this usage information.\n"
         << "  -s  --secret pattern  Secret key to import.\n"
         << "  -p  --public pattern  Public key to import.\n"
         << flush;
}


string passphrase(const string& hint, const string& info)
{
    cout << "hint: " << hint << "\ninfo: " << info << endl;
    return "pass";
}


int main(int argc, char **argv)
{
    try {

        const char* const short_options = "hs:p:";
        const struct option long_options[] = {
            { "help",      0, nullptr, 'h' },
            { "secret",    1, nullptr, 's' },
            { "public",    1, nullptr, 'p' },
            { nullptr,     0, nullptr,  0  }
        };

        string secret_pattern, public_pattern;
        int next_option;

        do {

            next_option = getopt_long(argc, argv, short_options,
                                      long_options, nullptr);
            switch(next_option) {

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

        } while(next_option != -1);

        if(secret_pattern.empty() && public_pattern.empty()) {
            print_help(argv[0]);
            return 0;
        }

        // Begin actual example

        crypto c;

        if(!secret_pattern.empty())
            cout << "Import secret key returned: " << boolalpha
                 << c.import_private_key(secret_pattern) << endl;

        if(!public_pattern.empty())
            cout << "Import public key returned: " << boolalpha
                 << c.import_public_key(public_pattern) << endl;

    } catch(const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}


/*
  Local Variables:
  mode: c++
  c-basic-offset: 4
  tab-width: 4
  c-indent-comments-syntactically-p: t
  c-tab-always-indent: t
  indent-tabs-mode: nil
  End:
*/

// vim:shiftwidth=4:autoindent:tabstop=4:expandtab:softtabstop=4
