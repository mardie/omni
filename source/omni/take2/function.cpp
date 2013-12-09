#include <omni/take2/function.hpp>
#include <omni/take2/context.hpp>
#include <omni/take2/type.hpp>
#include <omni/take2/block.hpp>
#include <omni/take2/statement.hpp>
#include <omni/take2/parameter.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Module.h>

/**
Initializes a function implementation with the given name, returnType, body and visibility.
@param name The name of this function.
@param returnType The return type of this function. Use a type constucted with the type_class omni::take2::type_class::t_void for functions that have no return value.
@param body The body (implementation) of this function.
@param isExported Specifies, whether this function is visible from outside the module it is defined in. @see isExported().
**/
omni::take2::function::function (std::string const & name,
                                 std::shared_ptr <type> returnType,
                                 std::shared_ptr <block> body,
                                 bool isExported) :
    function_prototype (name, returnType),
    _body (body),
    _isExported (isExported)
{
}

/**
Returns true, if this function is exported from the module it is defined in. Depending on what kind of module it is, exporting
has different meanings.
If isExported () returns false, the function is only locally visible in the module it is defined in.
**/
bool omni::take2::function::isExported () const
{
    return _isExported;
}

const std::shared_ptr <omni::take2::block> omni::take2::function::getBody () const
{
    return _body;
}

llvm::Function * omni::take2::function::llvmFunction (llvm::Module & llvmModule)
{
    if (_llvmFunction != nullptr) {
        return _llvmFunction;
    } else {
        llvm::FunctionType * funcType = llvmFunctionType ();

        llvm::GlobalValue::LinkageTypes linkageType;
        if (isExported ()) {
            // TODO: Handle static and dynamic libraries differently - if omni will have static libraries, that is.
            linkageType = llvm::GlobalValue::DLLExportLinkage;
        } else {
            linkageType = llvm::GlobalValue::InternalLinkage;
        }
        _llvmFunction = llvm::Function::Create (funcType, linkageType, getName (), & llvmModule);

        llvm::BasicBlock * body = llvm::BasicBlock::Create (getContext ()->llvmContext (), "__entry__", _llvmFunction);
        for (auto i : _body->getStatements ()) {
            i->llvmEmit (body);
        }
        return _llvmFunction;
    }
}
