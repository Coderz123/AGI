//#include <cypher-parser.h>
#include <errno.h>
#include <stdio.h>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

class Emotion
{
public:
	Emotion(){};
	Emotion(std::string name):_name(name){};
	virtual ~Emotion(){};

	int increase()
	{
		_intensity++;
		return _intensity;
	}

	int decrease()
	{
		_intensity--;
		return _intensity;
	}

	void print()
	{
		std::cout << "Emotion   :     " << _name << std::endl;
	}

	std::string toString()
	{
		std::string reslt{};
		return reslt;
	}

	std::string toHtml()
	{
		std::string reslt{};
		return reslt;
	}

	int _id = -1;
	int _oppositeId = -1;
	int _intensity = 0;
    std::string _name;
    std::string _description;
	std::vector<int> _neighbours;
	std::vector<std::tuple<std::string, int, int>> _memberEmotions;


};

