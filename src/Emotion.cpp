#include "Soul/Emotion/Emotion.h"

#include <sstream>

std::string Emotion::toString() const
{
    std::ostringstream result;
    result << _name;

    if (!_description.empty())
    {
        result << " — " << _description;
    }

    result << " (intensity " << _intensity << "/100)";
    return result.str();
}

std::string Emotion::toHtml() const
{
    return toString();
}
