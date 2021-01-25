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
         << "  -i  --input  filename Read input from file.\n"
         << "  -o  --output filename Write output to file.\n"
         << "  -s  --smime           Use S/MIME (default is PGP).\n"
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

        const char* const short_options = "hi:o:s";
        const struct option long_options[] = {
            { "help",      0, NULL, 'h' },
            { "output",    1, NULL, 'o' },
            { "input",     1, NULL, 'i' },
            { "smime",     0, NULL, 'i' },
            { NULL,        0, NULL,  0  }
        };

        string input_filename, output_filename;
        gpgme_protocol_t protocol = GPGME_PROTOCOL_OpenPGP;
        int next_option;

        do {

            next_option = getopt_long(argc, argv, short_options,
                                      long_options, NULL);
            switch(next_option) {

            case 'o':
                output_filename = optarg;
                break;

            case 'i':
                input_filename = optarg;
                break;

            case 's':
                protocol = GPGME_PROTOCOL_CMS;
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

        if(output_filename.empty() || input_filename.empty()) {
            print_help(argv[0]);
            return 0;
        }

        // Begin actual example

        file_data_buffer in(input_filename);
        file_data_buffer out(output_filename);

        crypto c(protocol);
        c.decrypt(passphrase, in, out);

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
