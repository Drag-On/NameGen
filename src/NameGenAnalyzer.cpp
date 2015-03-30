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
#include "DBGL/Core/Parsers/Properties.h"
#include "utility.h"

CharMaps g_characterMaps;
dbgl::Properties g_properties;

/**
 * @brief Analyzes a single line
 * @param line Line to analyze
 */
void analyzeLine(std::string const& line)
{
	// Start at each character of the line
	for (unsigned int i = 0; i < line.size(); i++)
	{
		std::string curString;
		// Iterate over all continuous character sequences
		for (int j = 0; j < g_properties.getIntValue("distMaxLength") && i + j < line.size(); j++)
		{
			curString += ::tolower(line[i + j]);
			g_characterMaps[j][curString]++;
		}
	}
}

/**
 * @brief Normalizes the character map such that all columns sum up to one
 */
void normalizeCharMap()
{
	for (unsigned int i = 0; i < g_characterMaps.size(); i++)
	{
		unsigned int sum = 0;
		for (auto it = g_characterMaps[i].begin(); it != g_characterMaps[i].end(); ++it)
			sum += it->second;
		for (auto it = g_characterMaps[i].begin(); it != g_characterMaps[i].end(); ++it)
		{
//			std::cout << it->first << ": " << it->second << std::endl;
			g_characterMaps[i][it->first] /= sum;
//			std::cout << it->first << ": " << it->second << std::endl;
		}
	}
}

/**
 * @brief Main application entry point
 * @return
 */
int main(int argc, const char* argv[])
{
	// Read in properties file
	if(!setupProperties(g_properties, argc, argv))
		return -1;
	// Setup character maps
	g_characterMaps.resize(g_properties.getIntValue("distMaxLength"));
	// Open file
	std::ifstream inFile;
	inFile.open(g_properties.getStringValue("inputFile"), std::ios::in);
	if (inFile.is_open())
	{
		// Seek to beginning
		inFile.seekg(0, std::ios::beg);
		// Scan whole file
		while (inFile.good())
		{
			std::string line;
			std::getline(inFile, line);
			analyzeLine(line);
		}
		inFile.close();
	}
	else
	{
		std::cout << "File " << g_properties.getStringValue("inputFile") << " could not be opened!" << std::endl;
		return -1;
	}
	// Normalize character map
	normalizeCharMap();
	// Write out character probability map
	writeCharMaps(g_properties.getStringValue("outputFile"), g_characterMaps);
	std::cout << "Done!" << std::endl;
	return 0;
}
