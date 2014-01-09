#ifndef OMNI_CORE_BITCAST_EXPRESSION_HPP
#define OMNI_CORE_BITCAST_EXPRESSION_HPP

#include <omni/core/core.hpp>
#include <omni/core/cast_expression.hpp>

namespace omni {
namespace core {

/**
A bitcast_expression casts one type to another, where both types have the same bit width.
The cast does not change any bits, but instead just reinterprets the given bits to be a value of the given target type.
bitcasts are dangerous and are usually only used internal in the compiler. Most likely, they will not surface in the omni language.
**/
class OMNI_CORE_API bitcast_expression : public cast_expression {
public:
    bitcast_expression (std::shared_ptr <expression> sourceExpression, std::shared_ptr <type> targetType);

    std::shared_ptr <type> getType () const override;

    llvm::Value * llvmValue (llvm::BasicBlock * llvmBasicBlock) override;

private:
    std::shared_ptr <expression> _sourceExpression;
    std::shared_ptr <type>       _targetType;
};
}
}

#endif // include guard
