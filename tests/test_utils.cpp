#include <omni/tests/test_utils.hpp>
#include <omni/take2/type.hpp>
#include <omni/take2/block.hpp>
#include <omni/take2/function.hpp>
#include <omni/take2/linkage_type.hpp>
#include <omni/take2/context.hpp>
#include <sstream>

/**
Emits a shared library file with the name fileBaseName.dll. The library exports a function "main" that calls the
function `func'. That way, it does not matter whether func's linkage_type is external or not.
Main automatically has the same return type as func. func and main may not take parameters.
@return The path of the created shared library file.
**/
boost::filesystem::path omni::tests::emitSharedLibraryWithFunction (std::shared_ptr <omni::take2::function> func,
                                                                    omni::tests::test_file_manager & testFileManager,
                                                                    std::string const & fileBaseName,
                                                                    std::string & functionName)
{
    static unsigned int counter = 0;
    using namespace omni::take2;
    // First, add a function that calls the wanted function.
    std::shared_ptr <type> returnType  (func->getReturnType ());
    std::shared_ptr <block> body (new block ());
    std::shared_ptr <expression> callExpression (new function_call_expression (func));
    std::shared_ptr <statement> returnStatement (new return_statement (callExpression));
    body->appendStatement (returnStatement);
    std::stringstream functionNameBuilder;
    functionNameBuilder << "main" << (++ counter);
    functionName = functionNameBuilder.str ();
    std::shared_ptr <function> caller (new function (functionName, returnType, body));
    caller->setLinkageType (linkage_type::external);
    context & c (* func->getContext ());
    c.addFunction (caller);
    std::string sharedLibraryName = testFileManager.getTestFileName (fileBaseName + ".dll").string ();
    c.emitSharedLibraryFile (sharedLibraryName);

//    BOOST_CHECK (boost::filesystem::exists(sharedLibraryName));
    return sharedLibraryName;
}
