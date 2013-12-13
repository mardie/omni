#include <omni/core/function.hpp>
#include <omni/core/block.hpp>
#include <omni/core/module.hpp>
#include <omni/core/context.hpp>
#include <omni/core/type.hpp>

#include <boost/test/unit_test.hpp>

/**
std::vector <std::shared_ptr <parameter>> getParameters () const; // TODO
const std::shared_ptr <block> getBody () const; // TODO
**/

BOOST_AUTO_TEST_SUITE (funcionTests)

BOOST_AUTO_TEST_CASE (ctor)
{
    using namespace omni::core;
    context c;
    module mod (c, "test");
    std::shared_ptr <type> returnType (type::sharedBasicType (c, type_class::t_signedInt));
    function func (mod, "hello", returnType, nullptr);
}

BOOST_AUTO_TEST_CASE (getReturnType)
{
    using namespace omni::core;
    context c;
    module mod (c, "test");
    std::shared_ptr <type> returnType (type::sharedBasicType (c, type_class::t_signedInt));
    function func (mod, "hello", returnType, nullptr);
    BOOST_CHECK (func.getReturnType ()->getTypeClass () == type_class::t_signedInt);
}

BOOST_AUTO_TEST_SUITE_END ();
