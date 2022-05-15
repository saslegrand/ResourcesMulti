#include "utils.hpp"

#include <imgui.h>
#include <iostream>
#include <algorithm>

namespace Utils
{
    bool hasPrefix(const std::string& str, const std::string& prefix)
    {
        // Check if the current string as the input prefix
        return str.size() >= prefix.size() &&
            str.compare(0, prefix.size(), prefix) == 0;
    }

    bool hasSuffix(const std::string& str, const std::string& suffix)
    {
        // Check if the current string as the input suffix
        return str.size() >= suffix.size() &&
            str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    bool isAnInteger(const std::string& str)
    {
        // Check if every char of the string is a digit
        return std::all_of(str.begin(), str.end(), [](char c)
            {
                return isdigit(c);
            });
    }

    bool isADecimal(const std::string& str)
    {
        // Return true if the string is decimal, check if it has one or less dot and sign, 

        int digitCount = 0, dotCount = 0, signCount = 0;

        // Check if every char of the string is a digit or a mathematical symbol
        for (char c : str)
        {
            if (c == ' ' || c == '\r') continue;

            if (isdigit(c))
            {
                digitCount++;
                continue;
            }

            if (c == '.')
            {
                dotCount++;
                continue;
            }

            if (c == '-' || c == '+')
            {
                signCount++;
                continue;
            }

            return false;
        }

        return digitCount > 0
            && signCount <= 1
            && dotCount <= 1;
    }

    void splitString(const std::string& input, const std::string& delimiter,
        std::string& output, const std::function<void()>& lambda)
    {
        // Loop over the sub-strings, and put them in the output string (to get access it after)
        size_t begin = 0;
        for (size_t end = 0; (end = input.find(delimiter, end)) != std::string::npos; end++)
        {
            output = input.substr(begin, end - begin);

            lambda();

            begin = end + 1;
        }

        output = input.substr(begin);

        lambda();
    }

    std::string getFileNameFromPath(const std::string& filePath)
    {
        return filePath.substr(filePath.find_last_of("/\\") + 1, filePath.size());
    }

    std::string getFileExtension(const std::string& filePath)
    {
        return filePath.substr(filePath.find_last_of("."), filePath.size());
    }

    std::string getDirectory(const std::string& filePath)
    {
        std::string fileName = getFileNameFromPath(filePath);

        return filePath.substr(0, filePath.size() - fileName.size());
    }

    float remap(float value, float from_min, float from_max, float to_min, float to_max)
    {
        return (value - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
    }

    std::string vecToStringDebug(const Core::Maths::vec2& toWrite)
    {
        return "x = " + std::to_string(toWrite.x) + ", y = " + std::to_string(toWrite.y);
    }

    std::string vecToStringDebug(const Core::Maths::vec3& toWrite)
    {
        return vecToStringDebug(toWrite.xy) + ", z = " + std::to_string(toWrite.z);
    }

    std::string vecToStringDebug(const Core::Maths::vec4& toWrite)
    {
        return vecToStringDebug(toWrite.xyz) + ", w = " + std::to_string(toWrite.w);
    }



    std::string vecToStringParsing(const Core::Maths::vec2& toWrite)
    {
        return std::to_string(toWrite.x) + " " + std::to_string(toWrite.y) + " ";
    }

    std::string vecToStringParsing(const Core::Maths::vec3& toWrite)
    {
        return vecToStringParsing(toWrite.xy) + std::to_string(toWrite.z) + " ";
    }

    std::string vecToStringParsing(const Core::Maths::vec4& toWrite)
    {
        return vecToStringParsing(toWrite.xyz) + std::to_string(toWrite.w) + " ";
    }

    std::string quatToStringParsing(const Core::Maths::quat& toWrite)
    {
        return std::to_string(toWrite.x) + " " + std::to_string(toWrite.y) + " " + std::to_string(toWrite.z) + " " + std::to_string(toWrite.w) + " ";
    }

    void selectImGuiString(std::string selectableLabel, std::string& strRef)
    {
        std::string _label = "   " + selectableLabel;
        const char* label = _label.c_str();

        if (ImGui::Selectable(label))
            strRef = selectableLabel;

        if (selectableLabel.compare(strRef) == 0)
        {
            ImGui::SameLine();
            ImGui::Text("   Selected");
        }
    }

    float clampLoop(float value, float min, float max)
    {
        float offset = max - min;

        if (value < min)
            return value + offset;
        if (value > max)
            return value - offset;

        return value;
    }

    Core::Maths::vec3 clampLoop(const Core::Maths::vec3& value, float min, float max)
    {
        return Core::Maths::vec3(clampLoop(value.x, min, max), clampLoop(value.y, min, max), clampLoop(value.z, min, max));
    }

    void imGuiTextFromString(const std::string& str)
    {
        const char* cStr = str.c_str();
        ImGui::Text(cStr);
    }

    std::string getTimeStrFromSeconds(float seconds)
    {
        int min = (int)(seconds / 60.f);
        seconds -= (float)(min * 60);

        return std::to_string(min) + " min and " + std::to_string(seconds) + " secs";
    }
}