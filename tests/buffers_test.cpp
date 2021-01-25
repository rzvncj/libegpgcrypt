#include <data_buffers.hpp>
#include <iostream>

using namespace egpgcrypt;
using namespace std;

int main()
{
    try {

        const std::string FILENAME = "test.out";

        cout << "\nFile write test";

        file_data_buffer fdb1(FILENAME);

        fdb1.write("file line 1\n");
        fdb1.write("file line 2\n");

        cout << " done\n\nFile read test follows\n--- BEGIN FILE ---\n";

        file_data_buffer fdb2(FILENAME);
        std::string      buffer;

        while (fdb2.read(buffer))
            cout << buffer;

        cout << "--- END FILE ---\n\nMemory write test";

        memory_data_buffer mdb1;

        mdb1.write("memory line 1\n");
        mdb1.write("memory line 2\n");

        if (mdb1.read(buffer))
            cerr << "This shouldn't work!\n";

        mdb1.seek(0, data_buffer::SET);

        cout << " done\n\nMemory read test follows\n--- BEGIN ---\n";

        while (mdb1.read(buffer))
            cout << buffer;

        cout << "--- END ---\n\nmdb1.content():\n" << mdb1.content() << endl;

    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}
