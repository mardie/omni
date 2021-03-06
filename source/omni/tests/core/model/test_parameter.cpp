#include <omni/core/model/parameter.hpp>
#include <omni/core/model/meta_info.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (parameterTests)

BOOST_AUTO_TEST_CASE (metaInfo)
{
    using namespace omni::core::model;

    meta_info & meta = parameter::getStaticMetaInfo ();
    BOOST_CHECK_EQUAL (meta.getName (), "parameter");
    BOOST_CHECK_EQUAL (meta.getParent (), & scope::getStaticMetaInfo ());
    BOOST_CHECK_EQUAL (meta.getChildCount (), 0u);
}

BOOST_AUTO_TEST_SUITE_END ();
