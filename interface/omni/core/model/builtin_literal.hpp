#ifndef OMNI_CORE_BUILTIN_LITERAL_HPP
#define OMNI_CORE_BUILTIN_LITERAL_HPP

#include <omni/core/core.hpp>
#include <omni/core/model/literal.hpp>
#include <omni/core/model/native_type_to_type_class.hpp>
#include <omni/core/not_implemented_error.hpp>

#include <memory>

namespace omni {
namespace core {
namespace model {

    /**
    builtin_literal can be used to define literals of builtin numeric types (char, short, int, double, float).
    **/
    template <typename T>
    class OMNI_CORE_API builtin_literal : public literal {
    public:
        explicit builtin_literal (omni::core::context & context, T value);

        domain getDomain () const override;

        static const type_class typeClass = native_type_to_type_class <T>::typeClass;

        virtual llvm::Value * llvmValue ();

    private:
        T _value;
    };
} // namespace model
} // namespace core
} // namespace omni

#endif