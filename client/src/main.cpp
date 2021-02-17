#include <iostream>

#include "Client.h"
#include "TAGPackage.h"
#include "TextColor.h"

using namespace std;

int main(int argc, char **argv) {
    my::Client client;

    if (client.connectToServer(argc, argv)) {
        cout << TC_BLUE << "Connect to " ;
        cout << argv[1] << ":" << argv[2] << TC_DEFAULT << endl;
        while (1) {
            string st;
//            cin >> st;
            getline(cin, st);
            my::TAGPackage package(my::LongInt<uint8_t>(to_string(st.length())), my::DATA_T::TEXT);
            package.wrapData(reinterpret_cast<const uint8_t*>(st.data()), st.length());
            client.sendToServer(package, my::DATA_T::TEXT);
        }
    } else {
        cerr << TC_RED << "Can't connect to server" << TC_DEFAULT << endl;
    }

    return 0;
}
