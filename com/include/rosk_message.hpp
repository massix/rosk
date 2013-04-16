/* Definition for a ROSK message, nothing more than a wrapped Protobuf message */

#pragma once

#include <google/protobuf/message.h>
#include <zmq.hpp>
#include <version1.pb.h>
#include <stdexcept>

namespace rosk {
	namespace com {
		class rosk_message 
		{
			public:
				// Default constructor: builds an empty rosk_message
				explicit rosk_message(std::string const & _message_type);

				// Build a socket-sendable rosk_message from another one
				rosk_message(std::string const & _message_type, 
						google::protobuf::Message const & _message);

				// Build a message from a zmq::message_t object (inaltered)
				rosk_message(zmq::message_t & _zmq_message);

				// Build a rosk_message from a socket, blocking in the recv
				// WARNING: at the end of the process, the socket might be modified
				rosk_message(zmq::socket_t & zmq_socket); 

				virtual ~rosk_message();

				// Accessors
				template <typename MessageType>
				MessageType const & get_internal_message()
				{
					build_internal_message<MessageType>();
					return dynamic_cast<MessageType&>(*_internal_message);
				}

				template <typename MessageType>
				MessageType & access_internal_message()
				{
					build_internal_message<MessageType>();
					_internally_modified = true;
					return dynamic_cast<MessageType&>(*_internal_message);
				}

				// Be able to send in ZMQ sockets
				void send_in_socket(zmq::socket_t & socket); // socket will be modified

				// Write modifications
				void commit();

			protected:
				std::string _serialized_in_string;
				std::string _message_type;
				rosk::com::base_message * _proto_message;
				google::protobuf::Message * _internal_message;
				bool _internally_modified;

			private:
				void construct_from_msg(zmq::message_t & _zmq_message);

				template <typename MessageType>
				void build_internal_message()
				{
					if (_proto_message == 0) throw std::runtime_error("Message not initialized");

					// Lazy accessor
					if (_internal_message == 0) {
						_internal_message = new MessageType();
						_internal_message->ParsePartialFromString(_proto_message->serialized_message());
					}
				}
		};
	}
}

