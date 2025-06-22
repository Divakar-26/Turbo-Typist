// WordManager.h
#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype>

class WordManager {
public:
    bool loadFromFile(const std::string &filename) {
        std::ifstream file(filename);
        std::string word;
        while (file >> word) {
            if (word.size() < 3 || word.size() > 10) continue;
            char first = std::tolower(word[0]);
            wordGroups[first].push_back(word);
        }
        return true;
    }

    std::string getUniqueWord(int maxLength = 10) {
        std::vector<char> keys;
        for (auto &[key, val] : wordGroups) {
            if (usedFirstLetters.find(key) != usedFirstLetters.end()) continue;
            bool hasValid = std::any_of(val.begin(), val.end(), [&](const std::string &w) {
                return w.size() <= static_cast<std::string::size_type>(maxLength);
            });
            if (hasValid) keys.push_back(key);
        }

        if (keys.empty()) return "";

        std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
        char selected = keys.front();
        auto &vec = wordGroups[selected];

        std::vector<std::string> candidates;
        for (const std::string &w : vec) {
            if (w.size() <= static_cast<std::string::size_type>(maxLength)) candidates.push_back(w);
        }

        if (candidates.empty()) return "";

        std::string chosen = candidates[rand() % candidates.size()];
        usedFirstLetters.insert(selected);
        return chosen;
    }

    void resetUsed() {
        usedFirstLetters.clear();
    }

private:
    std::unordered_map<char, std::vector<std::string>> wordGroups;
    std::unordered_set<char> usedFirstLetters;
};
