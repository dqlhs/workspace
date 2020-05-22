#pragma once

#define serialize_struct_mem_var(...) \
    virtual bool serialize(serializer *_to) const override { \
        return serialze_var(_to, __VA_ARGS__); \
    }


#define deserialize_struct_mem_var(...) \
    virtual bool deserialize(deserializer * _from) override { \
        return deserialize_var(_from, __VA_ARGS__); \
    }

#define serialize_and_deserialize_struct_mem_var(...) \
    serialize_struct_mem_var(__VA_ARGS__) \
    deserialize_struct_mem_var(__VA_ARGS__)

#define serialize_struct_mem_var_sub(super, ...) \
    virtual bool serialize(serializer *_to) const override { \
        return super::serialize(_to) && serialze_var(_to, __VA_ARGS__); \
    }

#define deserialize_struct_mem_var_sub(super, ...) \
    virtual bool deserialize(deserializer * _from) override { \
        return super::deserialize(_from) && deserialize_var(_from, __VA_ARGS__); \
    }

#define serialize_and_deserialize_struct_mem_var_sub(super, ...) \
    serialize_struct_mem_var_sub(super, __VA_ARGS__) \
    deserialize_struct_mem_var_sub(super, __VA_ARGS__)
