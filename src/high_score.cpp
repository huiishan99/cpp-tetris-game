#include "high_score.h"

#include <fstream>

int LoadHighScore(const std::string &path)
{
    std::ifstream file(path);
    int score = 0;
    if (!(file >> score) || score < 0)
    {
        return 0;
    }
    return score;
}

bool SaveHighScore(const std::string &path, int score)
{
    if (score < 0)
    {
        score = 0;
    }

    std::ofstream file(path, std::ios::trunc);
    if (!file)
    {
        return false;
    }

    file << score << '\n';
    return static_cast<bool>(file);
}
