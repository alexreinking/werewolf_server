#ifndef WEREWOLFSERVER_TCP_CONNECTION_HPP
#define WEREWOLFSERVER_TCP_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <cstdint>
#include <spdlog/spdlog.h>

#include "network/packet.hpp"

namespace ww {
    /**
     * @brief Send and receive data using TCP.
     * Each packet follows the following format: {[HEADER (2 BYTES)][CONTENT {String} (SIZE CODED IN HEADER)].}
     */
    class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
    public:
        tcp_connection() = delete;

        tcp_connection(boost::asio::io_context &context);

        void start();

        void stop();

        void write(std::string const &message);

        boost::asio::ip::tcp::socket &socket();

    private:
        // Read part is split into two processes:
        // 1. We read the header that contains the size of the incoming packet.
        //    It is coded on 2 Bytes
        // 2. We then read the amount of Bytes announced by the header
        void read_header();

        void handle_read_header(const boost::system::error_code &ec, std::size_t bytes_transferred);

        void read_body();

        void handle_read_body(const boost::system::error_code &ec, std::size_t bytes_transferred);

        void handle_write();

        void write(packet const &p);

        boost::asio::ip::tcp::socket m_socket;
        std::vector<char> m_body_buffer;
        std::array<char, header_size> m_header_buffer;
        header_size_type m_packet_size;
    };
}


#endif //WEREWOLFSERVER_TCP_CONNECTION_HPP
