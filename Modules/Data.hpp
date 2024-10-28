/*
** EPITECH PROJECT, 2023
** B-CPP-500-COT-5-1-rtype-fitzgerald.adechian
** File description:
** Serialize
*/

#ifndef HUFFMAN_COMPRESSION_HPP
    #define HUFFMAN_COMPRESSION_HPP
    #include <boost/asio.hpp>
    #include <cstdint>

enum class MsgType : uint8_t
{
    GetAllTotalPackets,
    TotalPackets,
    OK,
    KO,
// Game command start
    Info, // [entityid|state|drawableType|orientation|x|y|
    Stats, // |health|score|level|
    Fire,
    MoveRight,
    MoveLeft,
    Win,
    Loose,
    MoveUp,
    MoveDown,
// Game Command end
    UnvalidMessage,
    ConnectionRequire,
    ConnectionRequest, // |playerName|
    ConnectionRefused,
    ConnecionEstablished,
    AdminConnectionRequest, // |username password|
    AdminConnectionEstablished,
    AdminConnectionRefused,
// Room command start    
    CreateRoom,
    DeleteRoom,
    RoomFull,
    JoinRoom,
    LeaveRoom,
    ListRoom, // |roomId1|roomId2|...|
    ListRoomUser,
    NewRoomCreated,
    NewPlayerAddToRoom,
    RoomUserList, // |JoinedUser1 JoinedUser2 JoinedUser3|
    ListPlayerReady,
    GameStart,
    RoomSendMessage, // |message|
    RoomNewMessage, // |Message|
    RoomList,// |roomId|
    Ready,
// Room commnad end
// Admin command start
    DeconnectionRequest,
    AdminDeconnectionRequest,
    ListConnectedUser,
    ConnectedUsers, // |userId|userId|...|
    InfoUser, // |---|
    UserInfo, // |state|ip Username|
    InfoRoom, // |RoomId|
    RoomInfo, // |maxUser|nbReady|state|ipCreator|
    AdminListRoom,
    BanUser, // |userId| -> noResponse
    DeBan, // |userIp|
    ListUserBanned,
    UserBannedList,
    UserBanned,
    SETMAXROOM,
    SETMAXROOMUSER,
    SETCYCLE,
// Admin command end
    End,
};

namespace Net {
    struct Packet {
        MsgType id;
        std::vector<uint8_t> body;

        Packet(MsgType i = MsgType::OK) : id(i) {}

        size_t size() const
        {
            return body.size();
        }

        friend std::ostream& operator<<(std::ostream& os, const Packet& msg)
		{
			os << "ID:" << int(msg.id) << " Size:" << msg.body.size();
			return os;
		}

        template<typename DataType>
		friend Packet& operator<<(Packet& msg, const DataType& data)
		{
			size_t i = msg.body.size();
			msg.body.resize(msg.body.size() + sizeof(DataType));
			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
			return msg;
		}

        template<typename DataType>
		friend Packet& operator>>(Packet& msg, DataType& data)
		{
			size_t i = msg.body.size() - sizeof(DataType);
			std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
			msg.body.resize(i);
			return msg;
		}
    };


	struct ownedPacket
	{
		boost::asio::ip::udp::endpoint endpoint;
		Packet msg;
		friend std::ostream& operator<<(std::ostream& os, const ownedPacket& msg)
		{
			os << msg.msg;
			return os;
		}
	};

}


#endif /* !HUFFMAN_COMPRESSION_HPP */
