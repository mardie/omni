#include <omni/core/input/abstract_syntax_element.hpp>

omni::core::input::abstract_syntax_element::abstract_syntax_element () :
    _possibleSubstitutions ()
{
}

omni::core::input::abstract_syntax_element::abstract_syntax_element (std::vector <std::shared_ptr <omni::core::input::syntax_element>> possibleSubstitutions) :
    _possibleSubstitutions (possibleSubstitutions)
{
}

std::vector <omni::core::input::syntax_suggestion> omni::core::input::abstract_syntax_element::suggestImpl (std::string input, std::size_t templatePosition, std::set <syntax_element *> alreadyVisistedElements)
{
    if (alreadyVisistedElements.find (this) != alreadyVisistedElements.end ()) {
         return std::vector <omni::core::input::syntax_suggestion> ();
    }
    alreadyVisistedElements.insert (this);
    std::vector <omni::core::input::syntax_suggestion> result;
    for (auto i : _possibleSubstitutions) {
        auto suggestions = i->suggestImpl (input, templatePosition, alreadyVisistedElements);
        result.insert (result.end (), suggestions.begin (), suggestions.end ());
    }
    return result;
}

void omni::core::input::abstract_syntax_element::setPossibleSubstitutions (std::vector <std::shared_ptr <omni::core::input::syntax_element>> possibleSubstitutions)
{
    _possibleSubstitutions = possibleSubstitutions;
}

std::vector <std::shared_ptr <omni::core::input::syntax_element>> omni::core::input::abstract_syntax_element::getPossibleSubstitutions () const
{
    return _possibleSubstitutions;
}
