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
         << "  -i  --input filename  Read input from file.\n"
         << "  -a  --addresses list  Comma separated list of email addresses.\n"
         << "  -s  --smime           Use S/MIME (default is PGP).\n"
         << flush;
}


void split_string(const string& input, set<string>& output)
{
    string delimiters = " ,";
    size_t current;
    size_t next = -1;

    output.clear();

    do {
        current = next + 1;
        next = input.find_first_of(delimiters, current);
        output.insert(input.substr(current, next - current));

    } while (next != string::npos);
}


int main(int argc, char **argv)
{
    try {

        const char* const short_options = "hi:a:s";
        const struct option long_options[] = {
            { "help",      0, NULL, 'h' },
            { "input",     1, NULL, 'i' },
            { "addresses", 1, NULL, 'a' },
            { "smime",     0, NULL, 's' },
            { NULL,        0, NULL,  0  }
        };

        string input_filename, str_addresses;
        gpgme_protocol_t protocol = GPGME_PROTOCOL_OpenPGP;
        int next_option;

        do {

            next_option = getopt_long(argc, argv, short_options,
                                      long_options, NULL);
            switch(next_option) {

            case 'i':
                input_filename = optarg;
                break;

            case 'a':
                str_addresses = optarg;
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

        if(input_filename.empty() || str_addresses.empty()) {
            print_help(argv[0]);
            return 0;
        }

        // Begin actual example

        file_data_buffer in(input_filename);
        memory_data_buffer out;

        set<string> email_addresses;
        split_string(str_addresses, email_addresses);

        crypto c(protocol);
        c.sign(email_addresses, in, out);

        cout << out.content() << endl;

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
