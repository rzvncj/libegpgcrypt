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
         << "  -s  --sig filename    Read signature from file.\n"
         << "  -i  --input filename  File to check signature for.\n"
         << flush;
}


int main(int argc, char **argv)
{
    try {

        const char* const short_options = "hi:s:";
        const struct option long_options[] = {
            { "help",      0, nullptr, 'h' },
            { "input",     1, nullptr, 'i' },
            { "sig",       1, nullptr, 's' },
            { nullptr,     0, nullptr,  0  }
        };

        string input_filename, sig_filename;
        gpgme_protocol_t protocol = GPGME_PROTOCOL_OpenPGP;
        int next_option;

        do {

            next_option = getopt_long(argc, argv, short_options,
                                      long_options, nullptr);
            switch(next_option) {

            case 'i':
                input_filename = optarg;
                break;

            case 's':
                sig_filename = optarg;
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

        if(input_filename.empty() || sig_filename.empty()) {
            print_help(argv[0]);
            return 0;
        }

        // Begin actual example

        file_data_buffer in(input_filename);
        file_data_buffer sig(sig_filename);

        crypto c(protocol);

        cout << "Signature is "
            << (c.verify(sig, in) ? "valid" : "invalid")
            << endl;

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
