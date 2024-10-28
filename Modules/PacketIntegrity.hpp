/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** Serialize
*/

#ifndef HUFFMAN_COMPRESSION_HPP
    #define HUFFMAN_COMPRESSION_HPP
    #include <boost/asio.hpp>
    #include <vector>
    #include <cstdint>

static const int TIMEOUT_SECONDS = 5;

class ReliableUdpSocket : public INetWork {
    public:
        ReliableUdpSocket(boost::asio::io_service &io_service, unsigned short local_port) 
            : underlying_socket_(io_service, local_port), next_seq_num_(0), 
              next_expected_ack_(0), window_size_(10) {}

        void send(const Packet &packet, const IP &dest_ip, unsigned short dest_port) override {
            while((next_seq_num_ - next_expected_ack_) >= window_size_) {
                handle_acks();
            }
            std::string seq_num = std::to_string(next_seq_num_);
            Packet seq_packet(seq_num + ":" + packet.str_data);
            underlying_socket_.send(seq_packet, dest_ip, dest_port);
            unacknowledged_packets_[next_seq_num_] = packet;
            next_seq_num_++;
        }

        Packet fetch() override {
            Packet packet = underlying_socket_.fetch();            
            size_t separator_pos = packet.str_data.find(":");
            int seq_num = std::stoi(packet.str_data.substr(0, separator_pos));
            std::string data = packet.str_data.substr(separator_pos + 1);
            Packet ack_packet("ACK:" + std::to_string(seq_num));
            underlying_socket_.send(ack_packet, /*destination IP and port*/);
            return Packet(data);
        }
        bool available() override {}

    private:
        UdpSocket underlying_socket_;
        int next_seq_num_;
        int next_expected_ack_;
        int window_size_;
        std::map<int, Packet> unacknowledged_packets_;

    void handle_acks() {
        while(underlying_socket_.available()) {
            Packet packet = underlying_socket_.fetch();

            size_t separator_pos = packet.str_data.find(":");
            std::string prefix = packet.str_data.substr(0, separator_pos);

            if(prefix == "ACK") {
                int ack_seq_num = std::stoi(packet.str_data.substr(separator_pos + 1));

                unacknowledged_packets_.erase(ack_seq_num);
                packet_timers_.erase(ack_seq_num);
                if(ack_seq_num == next_expected_ack_) {
                    next_expected_ack_++;
                    while(unacknowledged_packets_.find(next_expected_ack_) == unacknowledged_packets_.end() && next_expected_ack_ < next_seq_num_) {
                        next_expected_ack_++;
                    }
                }
            }
        }

        auto now = std::chrono::steady_clock::now();
        for(auto it = packet_timers_.begin(); it != packet_timers_.end();) {
            if(std::chrono::duration_cast<std::chrono::seconds>(now - it->second).count() > TIMEOUT_SECONDS) {
                int expired_seq_num = it->first;
                Packet expired_packet = unacknowledged_packets_[expired_seq_num];
                std::string seq_num_str = std::to_string(expired_seq_num);
                Packet seq_packet(seq_num_str + ":" + expired_packet.str_data);
                underlying_socket_.send(seq_packet, /*IP and Port*/);
                it->second = now;
                ++it;
            }
            else {
                ++it;
            }
        }
    }
};

#endif /* !HUFFMAN_COMPRESSION_HPP */