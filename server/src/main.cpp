#include <iostream>
#include <chrono>

#include "Server.h"

using namespace std;

extern mutex g_mtx;

int main(int argc, char **argv) {
    my::Server server(argc, argv);
    string comand;

    std::ios_base::sync_with_stdio(true);

	server.start();
    while (1) {
//        this_thread::sleep_for(chrono::milliseconds(1000));
//        g_mtx.lock();
//        cout << "id = 0x" << hex << this_thread::get_id() << " main()" << endl;
//        g_mtx.unlock();
        cout << "> ";
        cout.flush();
        std::cin >> comand;
    }

	return 0;
}
