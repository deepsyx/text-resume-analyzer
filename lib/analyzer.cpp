#include <algorithm>
#include <sstream>
#include "analyzer.h"

struct Sentence
{
	std::string sentence;
	double weight;

	Sentence(std::string const& text) : sentence(text), weight(0) {};
};

// Finds the intersection between 2 sentences.
// Reusable with all text types and could return all number types
template<typename FwdIt0, typename FwdIt1, typename Comp, typename Num>
Num SmartAnalyzer::count_intersection(FwdIt0 beg0, FwdIt0 end0, FwdIt1 beg1, FwdIt1 end1, Comp less, Num n)
{
	while (beg0 != end0 && beg1 != end1)
	{
		if (less(*beg0, *beg1)) ++beg0;
		else if (less(*beg1, *beg0)) ++beg1;
		else
		{
			++n;
			++beg0;
			++beg1;
		}
	}

	return n;
}

// Finds intersection between 2 sentences and divide it to the average sentence length
template<typename FwdIt0, typename FwdIt1>
double SmartAnalyzer::intersection_weight(FwdIt0 beg0, FwdIt0 end0, FwdIt1 beg1, FwdIt1 end1)
{
	double const mid_size = 0.5 * (std::distance(beg0, end0) + std::distance(beg1, end1));
	double const intsc = count_intersection(beg0, end0, beg1, end1, std::less<>(), double());
	return intsc / mid_size;
}

//Splits text to sentences by dot
std::vector<Sentence> SmartAnalyzer::parseSentences(std::string const& text) 
{
	std::vector<Sentence> output;
	std::istringstream iss(text);
	std::string token;

	while (std::getline(iss, token, '.')) {
		output.push_back(Sentence(token));
	}

	return output;
}

// Removes punctuation from string
// The second arguments specifies if should remove dots also
std::string SmartAnalyzer::format(std::string text, bool includeDot)
{
	text.erase(std::remove_if(text.begin(), text.end(), [includeDot](char c) { 
		return c == ',' || c == '!' || c == '"' || (includeDot && c == '.' ); }), 
		text.end());

	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	return text;
}

// Takes vector of sorted sentences as argument
// Generates output based on the limit argument
// Appends sentences in decreasing weight order while output size is below the char limits
// If first sentences is over char limits, it's splitted by commas
std::string SmartAnalyzer::generateSentence(std::vector<Sentence> const& sentences, int limit)
{
	std::string output;
	std::size_t sentLen = sentences.size();

	for (std::size_t i = 0; i < sentLen; i++) {
		if (sentences[i].sentence.size() + output.size() > limit) {
			break;
		}
		output += sentences[i].sentence + ".";
	}

	if (output.empty()) {
		output = sentences[0].sentence;
		std::size_t pos = output.size();
		
		// Reverse find commas in the first sentence
		while ((pos = output.rfind(',', pos)) != std::string::npos && output.size() > limit) {
			output = output.substr(0, pos);
			pos--;
		}

		output += '...';
	}

	return output;
}

// Calculates the most important sentences in the text based on intersections with other sentences
std::string SmartAnalyzer::getSummary(std::string const& title, std::string const& text, int limit)
{
	std::vector<Sentence> sentences = this->parseSentences(text);
	int sentLen = sentences.size();

	// Calculates the intersection all to all sentences
	// TODO: optimize O(n*n) algorithm
	for (std::size_t i = 0; i < sentLen; i++)
	{
		// Add higher weight to the intersection with text title
		sentences[i].weight += 2 * this->intersection_weight(sentences[i].sentence.begin(), 
													   sentences[i].sentence.end(),
													   title.begin(),
													   title.end());
		
		for (std::size_t j = i + 1; j < sentLen; j++)
		{
			double const res = this->intersection_weight(sentences[i].sentence.begin(),
												   sentences[i].sentence.end(),
												   sentences[j].sentence.begin(),
												   sentences[j].sentence.end());
			sentences[i].weight += res;
			sentences[j].weight += res;
		}
	}

	// Sort the vector with sentences by weight
	std::sort(sentences.begin(), sentences.end(),
		[](const Sentence& a, const Sentence& b) {
			return a.weight > b.weight; }
	);
	
	return this->generateSentence(sentences, limit);
}