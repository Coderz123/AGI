#pragma once

#include <algorithm>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class Emotion
{
public:
    using MemberEmotion = std::tuple<std::string, int, int>;

    Emotion() = default;

    Emotion(int id,
            int oppositeId,
            std::string name,
            std::string description,
            std::vector<int> neighbours,
            std::vector<MemberEmotion> memberEmotions)
        : _id(id),
          _oppositeId(oppositeId),
          _name(std::move(name)),
          _description(std::move(description)),
          _neighbours(std::move(neighbours)),
          _memberEmotions(std::move(memberEmotions))
    {}

	int increase()
	{
		_intensity = std::min(_intensity + 1, 100);
		return _intensity;
	}

	int decrease()
	{
		_intensity = std::max(_intensity - 1, 0);
		return _intensity;
	}

	[[nodiscard]] std::string toString() const;
	[[nodiscard]] std::string toHtml() const;

	int _id = -1;
	int _oppositeId = -1;
	int _intensity = 0;
    std::string _name;
    std::string _description;
	std::vector<int> _neighbours;
	std::vector<MemberEmotion> _memberEmotions;


};
