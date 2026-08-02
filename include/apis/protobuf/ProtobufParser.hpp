#include "proto_define.hpp"

#include "base/ReelayParser.hpp>
#include "proto_mapper.hpp"

using InMsg = std::vector<uint8_t>;
using MonitorMsg = std::unordered_map<std::string, message_type>;

class ProtobufParser : public ReelayParser<InMsg, MonitorMsg> {
 public:
  explicit ProtobufParser(const std::string& protobuf_message_type)
      : mapper(protobuf_message_type)
  {
    init_protobufs();
  }

  MonitorMsg parse(const InMsg& in) {
    mapper.update(in);
  }

 private:
  proto_mapper mapper;
};
