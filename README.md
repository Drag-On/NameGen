# NameGen

This is a random name generator which tries to emulate the style of an arbitrary list of names. It analyzes the frequency of characters and character-sequences in the training data, and randomly outputs words with a similar distribution.

## Analyzer
Needs a configuration file called "properties.txt" with the following entries:
 - Maximum length of character sequences to analyze: distMaxLength=6
 - Filename of the list of words to analyze, separated by newlines: inputFile=list.txt
 - Filename to write output to: outputFile=out.txt
 
## NameGen
Needs a configuration file called "properties.txt" with the following entries:
 - Letter table to use: letterTable=out.txt
 - Amount of names to generate: amountOfWords=10
 - Minimum length of generated names: minLetters=3
 - Maximum length of generated names: maxLetters=10
 - Capitalize first letter: capFirst=true
 - Use the n last characters to define the next character: priorLength=2

Note: The larger the option for 'priorLength', the greater the similarity of the generated names with the initial list, however this will also limit the amount of unique names that can be generated.
