#include "stdafx.h"
#include "meParser.h"

int FirstCharIndex(std::string str, int startIndex)
{
	for (size_t i = startIndex; i < str.length(); i++)
	{
		if (str[i] != ' ' && str[i] != '\t') { return (int)i; }
	}
	return -1;
}

int GetWordLeng(std::string str, int startIndex)
{
	int worldLeng = 0;
	for (size_t i = startIndex; i < str.length(); i++)
	{
		if (str[i] == ' ' || str[i] == '\t') { break; }
		else { worldLeng++; }
	}
	return worldLeng;
}

void Parser(std::string str, std::vector<std::string>& strings)
{
	strings.clear();
	int charIndex = 0;
	int wordLeng = 0;
	for (size_t i = 0; i < 50; i++)
	{
		charIndex = FirstCharIndex(str, charIndex);//the char index tracks where we are in the input string
		wordLeng = GetWordLeng(str, charIndex);
		if (charIndex == -1 || wordLeng == -1) { break; }
		strings.push_back(str.substr(charIndex, wordLeng));
		charIndex += wordLeng;
		FirstCharIndex(str, charIndex);
	}
}
