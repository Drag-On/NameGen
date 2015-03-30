//////////////////////////////////////////////////////////////////////
/// NameGen
///
/// Copyright (c) 2015 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include "DBGL/Core/Parsers/Properties.h"
#include "utility.h"

unsigned int g_seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine g_randGenerator(g_seed);
dbgl::Properties g_properties;
CharMaps g_characterMaps;

std::string pickNextLetter(std::string const& current, double probability)
{
	unsigned int priorLength = std::min(static_cast<unsigned int>(g_properties.getIntValue("priorLength")),
			current.length());
	if(priorLength >= g_characterMaps.size())
		return "";
	std::string prior = current.substr(current.length() - priorLength);
	// Ensure lowercase
	std::transform(prior.begin(), prior.end(), prior.begin(), ::tolower);
	// Iterate over all possible characters
	double curProb = 0;
	for (auto it = g_characterMaps[0].begin(); it != g_characterMaps[0].end(); ++it)
	{
		std::string searchStr = prior + it->first;
		curProb += g_characterMaps[priorLength][searchStr];
	}
	probability *= curProb;
	curProb = 0;
	for (auto it = g_characterMaps[0].begin(); it != g_characterMaps[0].end(); ++it)
	{
		std::string searchStr = prior + it->first;
		curProb += g_characterMaps[priorLength][searchStr];
		if (curProb > 0 && curProb >= probability)
			return it->first;
	}
	return "";
}

/**
 * @brief Main application entry point
 * @return
 */
int main(int argc, const char* argv[])
{
	// Read in properties file
	if (!setupProperties(g_properties, argc, argv))
		return -1;
	// Read in specified character table
	if (!readCharMaps(g_properties.getStringValue("letterTable"), g_characterMaps))
		return -1;
	// Construct random distributions
	std::uniform_int_distribution<int> wordLengthDistribution(g_properties.getIntValue("minLetters"),
			g_properties.getIntValue("maxLetters"));
	std::uniform_real_distribution<float> probabilityDistribution(0, 1);
	// Iterate as often as there are names to generate
	for (unsigned int i = 0; i < static_cast<unsigned int>(g_properties.getIntValue("amountOfWords")); i++)
	{
		std::string word;
		// Pick length of the current word
		unsigned int wordLength = wordLengthDistribution(g_randGenerator);
		// Pick starting letter
		word += pickNextLetter("", probabilityDistribution(g_randGenerator));
		// Capitalize if needed
		if (g_properties.getBoolValue("capFirst"))
			std::transform(word.begin(), word.end(), word.begin(), ::toupper);
		// Generate remaining letters based on data in the character table
		for (unsigned int j = 0; j < wordLength - 1; j++)
		{
			// Pick characters in sequence
			auto nextLetter = pickNextLetter(word, probabilityDistribution(g_randGenerator));
			if (nextLetter.empty())
				break;
			else
				word += nextLetter;
		}
		// Output the generated word
		std::cout << word << std::endl;
	}
	return 0;
}
