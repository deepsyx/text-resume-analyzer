#include <set>
#include <vector>

struct Sentence;

class SmartAnalyzer
{
private:
	// Takes vector of sorted sentences and creates a string from it, based on char limits
	std::string SmartAnalyzer::generateSentence(std::vector<Sentence> const& sentences, int limit);

	//Splits text to sentences by dot
	std::vector<Sentence> SmartAnalyzer::parseSentences(std::string const& text);

	// Finds the intersection between 2 sentences
	template<typename FwdIt0, typename FwdIt1, typename Comp, typename Num>
	Num count_intersection(FwdIt0 beg0, FwdIt0 end0, FwdIt1 beg1, FwdIt1 end1, Comp less, Num n);

public:
	SmartAnalyzer() {}

	// Finds the intersection between 2 sentences and divide it to the average sentence length
	template<typename FwdIt0, typename FwdIt1>
	double intersection_weight(FwdIt0 beg0, FwdIt0 end0, FwdIt1 beg1, FwdIt1 end1);

	// Removes punctuation from string
	std::string format(std::string text, bool includeDot);

	// Calculates the most important sentences in the text based on intersections with other sentences
	// Usage:
	//			SmartAnalyzer a;
	//			std::cout << a.getSummary(title, content, 5 * 36);
	//
	std::string getSummary(std::string const& title, std::string const& text, int limit);
};