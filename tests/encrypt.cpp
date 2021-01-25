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
         << "  -o  --output filename Write output to file.\n"
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

        const char* const short_options = "hi:o:a:s";
        const struct option long_options[] = {
            { "help",      0, nullptr, 'h' },
            { "output",    1, nullptr, 'o' },
            { "input",     1, nullptr, 'i' },
            { "addresses", 1, nullptr, 'a' },
            { "smime",     0, nullptr, 's' },
            { nullptr,     0, nullptr,  0  }
        };

        string input_filename, output_filename, str_addresses;
        gpgme_protocol_t protocol = GPGME_PROTOCOL_OpenPGP;
        int next_option;

        do {

            next_option = getopt_long(argc, argv, short_options,
                                      long_options, nullptr);
            switch(next_option) {

            case 'o':
                output_filename = optarg;
                break;

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

        if(output_filename.empty() || input_filename.empty()
           || str_addresses.empty()) {
            print_help(argv[0]);
            return 0;
        }

        // Begin actual example

        file_data_buffer in(input_filename);
        file_data_buffer out(output_filename);

        set<string> email_addresses;
        set<string> expired_emails;
        split_string(str_addresses, email_addresses);

        crypto c(protocol);
        c.encrypt(email_addresses, expired_emails, in, out);

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
