#ifndef OMNI_CORE_INVALID_ARGUMENT_ERROR_HPP
#define OMNI_CORE_INVALID_ARGUMENT_ERROR_HPP

#include <omni/core/core.hpp>

#include <stdexcept>
#include <string>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4275)
#endif

namespace omni {
namespace core {

    class OMNI_CORE_API invalid_argument_error : public std::invalid_argument {
    public:
        invalid_argument_error (std::string const & fileName, std::string const & functionName, int lineNumber, std::string argumentName, std::string const & errorMessage);
    };

} // namespace core
} // namespace omni

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

#endif // include guard
