#ifndef OMNI_CORE_SYNTAX_ELEMENT_HPP
#define OMNI_CORE_SYNTAX_ELEMENT_HPP

#include <omni/core/core.hpp>
#include <omni/core/input/syntax_suggestion.hpp>

#include <vector>
#include <memory>

namespace omni {
namespace core {
namespace input {
class template_element;

/**
**/
class OMNI_CORE_API syntax_element {
public:
    virtual std::shared_ptr <template_element> templatElementAt (std::size_t templatePosition);
    virtual std::size_t templateElementCount () const;

    virtual std::vector <syntax_suggestion> suggest (std::string input, std::size_t templateIndex = 0u) = 0;
};

} // namespace input
} // namespace core
} // namespace omni

#endif // include guard
