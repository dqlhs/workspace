#pragma once

namespace hsae {
namespace oxygen {

class deserializer;

class deserializable {
public:
    virtual ~deserializable() { }
    virtual bool deserialize(deserializer *_from) = 0;
};


} // namespace oxygen
} // namespace hsae
