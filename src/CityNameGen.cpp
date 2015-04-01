//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2015 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

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
std::vector<std::string> g_preWords;
std::vector<std::string> g_words;
std::vector<std::string> g_postWords;

/**
 * @brief Reads a file into a container line-wise
 * @param file
 * @param container
 * @return
 */
bool readFile(std::string const& file, std::vector<std::string>& container)
{
	// Open file
	std::ifstream inFile;
	inFile.open(file, std::ios::in);
	if (inFile.is_open())
	{
		// Seek to beginning
		inFile.seekg(0, std::ios::beg);
		// Scan whole file
		while (inFile.good())
		{
			std::string line;
			std::getline(inFile, line);
			if (line.empty())
				continue;
			container.push_back(line);
		}
		inFile.close();
		return true;
	}
	return false;
}

/**
 * @brief Main application entry point
 * @return
 */
int main(int argc, const char* argv[])
{
	// Set locale
	std::locale::global(std::locale(""));
	// Read in properties file
	if (!setupProperties(g_properties, argc, argv))
		return -1;
	// Read in pre words
	if (!readFile(g_properties.getStringValue("preList"), g_preWords))
		std::cout << "Unable to read file " << g_properties.getStringValue("preList") << std::endl;
	// Read in words
	if (!readFile(g_properties.getStringValue("wordList"), g_words))
		std::cout << "Unable to read file " << g_properties.getStringValue("wordList") << std::endl;
	// Read in post words
	if (!readFile(g_properties.getStringValue("postList"), g_postWords))
		std::cout << "Unable to read file " << g_properties.getStringValue("postList") << std::endl;
	// Generate random city names
	std::uniform_int_distribution<int> preDist(0, g_preWords.size() - 1);
	std::uniform_int_distribution<int> wordDist(0, g_words.size() - 1);
	std::uniform_int_distribution<int> postDist(0, g_postWords.size() - 1);
	std::cout << "Showing " << g_properties.getIntValue("amountOfWords") << " of "
			<< g_words.size() * g_postWords.size() << " possible names (not including descriptions):" << std::endl << std::endl;
	for (unsigned int i = 0; i < static_cast<unsigned int>(g_properties.getIntValue("amountOfWords")); i++)
	{
		std::string name;
		if (!g_preWords.empty())
			name += g_preWords[preDist(g_randGenerator)];
		if (!g_words.empty())
			name += " " + g_words[wordDist(g_randGenerator)];
		if (!g_postWords.empty())
			name += g_postWords[postDist(g_randGenerator)];
		std::cout << encodeString(name) << std::endl;
	}
	return 0;
}
