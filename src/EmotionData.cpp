#include "Soul/Emotion/EmotionData.h"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace
{
using rapidjson::Value;

const Value& requireMember(const Value& object, const char* name, rapidjson::Type type)
{
    if (!object.IsObject() || !object.HasMember(name) || object[name].GetType() != type)
    {
        throw std::runtime_error(std::string("Invalid or missing emotion field: ") + name);
    }
    return object[name];
}

int requireBoundedInt(const Value& object, const char* name, int minimum, int maximum)
{
    const auto& value = requireMember(object, name, rapidjson::kNumberType);
    if (!value.IsInt() || value.GetInt() < minimum || value.GetInt() > maximum)
    {
        throw std::runtime_error(std::string("Emotion field is out of range: ") + name);
    }
    return value.GetInt();
}
} // namespace

std::vector<Emotion> loadEmotions(const std::filesystem::path& path)
{
    std::ifstream input(path);
    if (!input)
    {
        throw std::runtime_error("Unable to open emotion data: " + path.string());
    }

    rapidjson::IStreamWrapper stream(input);
    rapidjson::Document document;
    document.ParseStream(stream);
    if (document.HasParseError())
    {
        throw std::runtime_error(
            "Invalid emotion JSON at offset " + std::to_string(document.GetErrorOffset()) +
            ": " + rapidjson::GetParseError_En(document.GetParseError()));
    }

    const auto& categories = requireMember(document, "Categories", rapidjson::kArrayType);
    std::vector<Emotion> result;
    result.reserve(categories.Size());
    std::unordered_set<int> ids;

    for (const auto& category : categories.GetArray())
    {
        const int id = requireBoundedInt(category, "id", 0, 1'000'000);
        const int oppositeId = requireBoundedInt(category, "oppositeId", 0, 1'000'000);
        if (!ids.insert(id).second)
        {
            throw std::runtime_error("Duplicate emotion category id: " + std::to_string(id));
        }

        const auto& name = requireMember(category, "name", rapidjson::kStringType);
        const auto& description = requireMember(category, "description", rapidjson::kStringType);
        const auto& neighboursValue = requireMember(category, "neighbours", rapidjson::kArrayType);
        if (neighboursValue.Size() != 2)
        {
            throw std::runtime_error("Emotion category must have exactly two neighbours: " +
                                     std::string(name.GetString()));
        }

        std::vector<int> neighbours;
        neighbours.reserve(2);
        for (const auto& neighbour : neighboursValue.GetArray())
        {
            if (!neighbour.IsInt() || neighbour.GetInt() < 0)
            {
                throw std::runtime_error("Invalid neighbour for emotion: " +
                                         std::string(name.GetString()));
            }
            neighbours.push_back(neighbour.GetInt());
        }

        const auto& membersValue = requireMember(category, "emotions", rapidjson::kArrayType);
        std::vector<Emotion::MemberEmotion> members;
        members.reserve(membersValue.Size());
        for (const auto& member : membersValue.GetArray())
        {
            const auto& memberName = requireMember(member, "emotion", rapidjson::kStringType);
            const int minimum = requireBoundedInt(member, "min", 0, 100);
            const int maximum = requireBoundedInt(member, "max", 0, 100);
            if (minimum > maximum)
            {
                throw std::runtime_error("Emotion range minimum exceeds maximum: " +
                                         std::string(memberName.GetString()));
            }
            members.emplace_back(memberName.GetString(), minimum, maximum);
        }

        result.emplace_back(id,
                            oppositeId,
                            name.GetString(),
                            description.GetString(),
                            std::move(neighbours),
                            std::move(members));
    }

    std::unordered_map<int, const Emotion*> byId;
    for (const auto& emotion : result)
    {
        byId.emplace(emotion._id, &emotion);
    }
    for (const auto& emotion : result)
    {
        const auto opposite = byId.find(emotion._oppositeId);
        if (opposite == byId.end() || opposite->second->_oppositeId != emotion._id)
        {
            throw std::runtime_error("Invalid opposite relationship for emotion: " + emotion._name);
        }
        for (const int neighbour : emotion._neighbours)
        {
            if (neighbour == emotion._id || !byId.contains(neighbour))
            {
                throw std::runtime_error("Invalid neighbour relationship for emotion: " + emotion._name);
            }
        }
    }

    return result;
}
