#include <omni/take2/literal.hpp>

omni::take2::literal::literal (std::shared_ptr <type> type) :
    _type (type)
{
}

omni::take2::literal::~ literal ()
{
}

std::shared_ptr <omni::take2::type> omni::take2::literal::getType ()
{
    return _type;
}

const std::shared_ptr <omni::take2::type> omni::take2::literal::getType () const
{
    return _type;
}