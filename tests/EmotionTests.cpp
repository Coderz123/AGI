#include "Body/Action/verbs.h"
#include "Soul/Emotion/EmotionData.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

#ifndef AGI_SOURCE_DIR
#define AGI_SOURCE_DIR "."
#endif

namespace
{
void require(bool condition, const std::string& message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}
} // namespace

int main()
{
    try
    {
        const auto emotions = loadEmotions(
            std::filesystem::path{AGI_SOURCE_DIR} / "data/Soul/EmotionData.json");

        require(emotions.size() == 16, "expected 16 emotion categories");
        require(emotions.front()._name == "Joy", "expected Joy to be the first category");
        require(emotions.front()._oppositeId == 1, "expected Sadness to oppose Joy");
        require(emotions.front()._memberEmotions.size() == 3,
                "expected Joy to contain three intensity bands");

        Emotion bounded;
        require(bounded.decrease() == 0, "intensity must not fall below zero");
        for (int index = 0; index < 101; ++index)
        {
            bounded.increase();
        }
        require(bounded._intensity == 100, "intensity must not exceed 100");
        require(!emotions.front().toString().empty(), "emotion text must not be empty");

        const std::unordered_set<std::string> uniqueActions(actions.begin(), actions.end());
        require(uniqueActions.size() == actions.size(), "action vocabulary must not contain duplicates");
    }
    catch (const std::exception& error)
    {
        std::cerr << "Test failure: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
