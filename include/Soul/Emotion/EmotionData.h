#pragma once

#include "Emotion.h"

#include <filesystem>
#include <vector>

std::vector<Emotion> loadEmotions(const std::filesystem::path& path);
