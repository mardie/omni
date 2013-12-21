#ifndef OMNI_CORE_BUILTIN_LITERAL_HPP
#define OMNI_CORE_BUILTIN_LITERAL_HPP

#include <omni/core/core.hpp>
#include <omni/core/literal.hpp>
#include <omni/core/not_implemented_error.hpp>
#include <omni/core/native_type_to_type_class.hpp>

#include <memory>

namespace omni {
namespace core {
    class context;

    template <typename T>
    class OMNI_CORE_API builtin_literal : public literal {
    public:
        explicit builtin_literal (context & context, T value);

        static const type_class typeClass = native_type_to_type_class <T>::typeClass;

        virtual llvm::Value * llvmValue ();

    private:
        T _value;
    };
}
}

#endif
