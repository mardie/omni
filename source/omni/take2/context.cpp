#include <omni/take2/context.hpp>
#include <omni/take2/function.hpp>
#include <omni/take2/already_exists_error.hpp>
#include <omni/take2/context_part.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/PassManager.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Analysis/Verifier.h>
#include "llvm/Support/raw_ostream.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/lexical_cast.hpp>

/**
Finds any part of this context by it's unique id. The part has to be added to this context before it can be found.
This happens when the part is created using one of the create...-functions or the part has been added via one of the add...-functions.
@param id The id of the part that should be returned. This should not be an invalid id.
@return The part with the id, if such has been added to the context. A null-shared_ptr is returned, if no such part exists in this context.
**/
std::shared_ptr <omni::take2::context_part> omni::take2::context::findPartById (omni::take2::id id)
{
    id_to_parts_map & m (_parts [id.getDomain ()]);
    id_to_parts_map::iterator i = m.find (id.getId ());
    if (i != m.end ()) {
        return i->second;
    } else {
        return std::shared_ptr <context_part> ();
    }
}

/**
Create a new, unique id for the domain `domain'.
@param domain The domain that the id should be valid for.
@return A new, unique id that can be used to identify newly created parts for this context.
**/
omni::take2::id omni::take2::context::createId (omni::take2::domain domain)
{         
    static unsigned int counter = 0;
    //static boost::uuids::random_generator generator;
    std::string strId;
    do {
        //boost::uuids::uuid uuid = generator ();
        std::stringstream str;
        str << ++ counter;
        //strId = boost::lexical_cast <std::string> (uuid);
        strId = str.str ();
    } while (findPartById (id (domain, strId)).get () != nullptr);
    return id (domain, strId);
}

/**
Creates a new function object for the function with the name `name', adds it to this context and returns it.
Calling createFunction is the same as creating a new function with this context as the first paramter and then calling addFunction().
That's why this function will automatically be assigned a new id.
@param name The name of the function. There may not be a function with the same name in the context, otherwise an already_exits_error exception will be thrown.
@exception already_exists_error Is thrown when a function with the name `name' already exists in this context.
**/
std::shared_ptr <omni::take2::function> omni::take2::context::createFunction (std::string const & name, std::shared_ptr <type> returnType, std::shared_ptr <block> body)
{
    std::shared_ptr <function> result (new function (* this, name, returnType, body));
    addFunction (result);
    return result;
}

/**
Adds the function `function' to this context, if there is not already another function with the same name.
`function' will automatically be assigned a new id before it is added to this context.
@param function The function that should be added to this context.
@exception already_exists_error Is thrown when a function with the same name as `function's name already exists in this context.
**/
void omni::take2::context::addFunction (std::shared_ptr <omni::take2::function> function)
{
    if (findFunctionByName (function->getName ()).get () != nullptr) {
        throw already_exists_error (domain::function, function->getName ());
    }
    id newFunctionId = createId (domain::function);
    function->setId (newFunctionId);
    id_to_parts_map & functionMap (_parts [domain::function]);
    functionMap [newFunctionId.getId ()] = function;
}

/**
Returns the function with the name `name', if such a function is part of this context.
Only functions that were created using createFunction or were added via addFunction are part of this context.
@param The name of the function that should be returned. Should not be empty.
@return The function with the name `name' that has previously been added to this context.
**/
std::shared_ptr <omni::take2::function> omni::take2::context::findFunctionByName (std::string const & name)
{
    id_to_parts_map & functionMap (_parts [domain::function]);
    auto found = std::find_if (functionMap.begin (), functionMap.end (), [name] (std::pair <std::string, std::shared_ptr <context_part>> f) -> bool {
        return f.second->getName () == name;
    });
    if (found != functionMap.end ()) {
        return std::dynamic_pointer_cast <function> (found->second);
    } else {
        return std::shared_ptr <omni::take2::function> ();
    }
}

/**
Removes the function `function' from this context. This only has an effect if `function' was previously added to this context by creating it using
createFunction or adding it via addFunction.
@param function The function to be removed from this context.
@return true, if `function' was part of this context and has been removed. false, if `function' was not found.
**/
bool omni::take2::context::removeFunction (std::shared_ptr <omni::take2::function> function)
{
    id_to_parts_map & functionMap (_parts [domain::function]);
    auto found = std::find_if (functionMap.begin (), functionMap.end (), [function] (std::pair <std::string, std::shared_ptr <context_part>> f) -> bool {
        return f.second == function;
    });
    if (found != functionMap.end ()) {
        functionMap.erase (found);
        return true;
    } else {
        return false;
    }
}

/**
Sets the entry point for this context.
Not implemented yet.
TODO - this should be part of a module, not the whole context.
**/
void omni::take2::context::setEntryPoint (std::shared_ptr <function> function)
{
    _entryPoint = function;
}

/**
Emits llvm IR language code to the standard output.
@param fileName Currently ignored.
**/
void omni::take2::context::emitAssemblyFile (std::string const & fileName)
{
    llvm::Module module ("test", llvm::getGlobalContext ());
    
    for (auto f : _parts [domain::function]) {
        function & func = * std::dynamic_pointer_cast <function> (f.second);
        func.llvmFunction (module);
    }

    llvm::verifyModule (module, llvm::PrintMessageAction);

    llvm::PassManager pm;
    pm.add (llvm::createPrintModulePass(& llvm::outs()));
    pm.run (module);
}