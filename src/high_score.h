#pragma once

#include <string>

int LoadHighScore(const std::string &path);
bool SaveHighScore(const std::string &path, int score);
