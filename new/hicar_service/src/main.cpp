#include "log/oxygen_log.h"
#include "utility/format.h"
#include "socket/tcp_server_channel.h"

void recv_handler(const uint8_t * _data, size_t _size)
{
    INFO("%s", hsae::oxygen::hex(_data, _size).c_str());
}

void error_handler(const hsae::tcpserver_status & status)
{
    INFO("%d", status);
}

int main(int argc, char * argv[])
{
    hsae::tcpserver_impl impl_8080(8080);
    hsae::tcpserver_impl impl_8000(8000);

    impl_8080.set_recv_handler(recv_handler);
    impl_8080.set_status_handler(error_handler);

    impl_8000.set_recv_handler(recv_handler);
    impl_8000.set_status_handler(error_handler);

    std::thread send_thread ([&] () {
        for (;;) {
            impl_8000.send_data((const uint8_t *)"hello", 5);
            impl_8080.send_data((const uint8_t *)"hello", 5);
            sleep(2);
        }
    });

    pause();

    return 0;
}
