#pragma once

#include <string>
#include <functional>

#include "maths.hpp"

namespace Utils
{
    // Check if the input string contains the prefix
    bool hasPrefix(const std::string& str, const std::string& prefix);

    // Check if the input string contains the suffix
    bool hasSuffix(const std::string& str, const std::string& suffix);

    // Check if the input string is an integer
    bool isAnInteger(const std::string& str);

    // Check if the input string is decimal
    bool isADecimal(const std::string& str);

    // Split the input string with the delimiter
    void splitString(const std::string& input, const std::string& delimiter,
        std::string& output, const std::function<void()>& lambda);

    float remap(float value, float from_min, float from_max, float to_min, float to_max);

    // Return the name of the file from the input filepath
    std::string getFileNameFromPath(const std::string& filePath);

    std::string getFileExtension(const std::string& filePath);

    // Return the directory of the file from the input filepath
    std::string getDirectory(const std::string& filePath);

    std::string vecToStringDebug(const Core::Maths::vec2& toWrite);

    std::string vecToStringDebug(const Core::Maths::vec3& toWrite);

    std::string vecToStringDebug(const Core::Maths::vec4& toWrite);

    std::string vecToStringParsing(const Core::Maths::vec2& toWrite);

    std::string vecToStringParsing(const Core::Maths::vec3& toWrite);

    std::string vecToStringParsing(const Core::Maths::vec4& toWrite);

    std::string quatToStringParsing(const Core::Maths::quat& toWrite);

    void selectImGuiString(std::string selectableLabel, std::string& strRef);

    void imGuiTextFromString(const std::string& str);

    float clampLoop(float value, float min, float max);

    Core::Maths::vec3 clampLoop(const Core::Maths::vec3& value, float min, float max);

    std::string getTimeStrFromSeconds(float seconds);

    template <typename T>
    int sign(T value)
    {
        return (T(0) < value) - (value < T(0));
    }
}