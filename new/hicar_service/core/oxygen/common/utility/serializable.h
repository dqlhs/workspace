#pragma once

namespace hsae {
namespace oxygen {

class serializer;

/**
 * Abstract base class for element that can be serialized.
 */
class serializable {
public:
    virtual ~serializable() {}

    /**
     * \brief serialize the content of the object
     */
    virtual bool serialize(serializer *_to) const = 0;
};

} // namespace oxygen
} // namespace hsae
