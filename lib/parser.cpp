#include <vector>
#include "parser.h"
#include "analyzer.h"

enum Tags {
	Closing,
	Paragraph,
	Heading
};

// Accepts char of string and returns the corresponding opening/closing tag
int tagDetect(std::string const& ptr){
	if (ptr.front() == '/') {
		return Tags::Closing;
	}

	if (ptr.front() == 'p') {
		return Tags::Paragraph;
	}

	if (ptr[1] == '2' || ptr[1] == '3')
		return Tags::Heading;

	return -1;
}

SmartAnalyzer analyzer;

// Parses xHTML string without nested tags to object with data and content properties
// Example usage:
//     Local<Array> summarized = Parser("big html data", "Data's title", 5 * 35)
// Note that the last parameter is the length of the chars, not number of words
Local<Array> Parser(std::string & input, std::string const& title, int sentenceLimit, bool analyzerFlag)
{
	Local<Array> nodes = Array::New();
	Local<Object> node_obj;

	std::size_t pos = 0;
	std::size_t closePos = -1;
	std::size_t openPos = -1;

	int tagID = -1;
	int lastTag = -1;
	int lastAppended = -1;

	// Output array iterator
	int id = 0;

	while ((pos = input.find('<', pos)) != std::string::npos) {
		pos++;
		switch (tagID = tagDetect(&input[pos])) {

		case Tags::Closing:

			// determine what kind of closing tag is
			tagID = tagDetect(&input[pos + 1]);

			// Is this the closure of the last tag
			if (tagID == lastTag && (pos - openPos > MIN_TEXT_LENGTH || lastTag != Tags::Paragraph)) {

				// If two tags of same kind are used in sequence skip second
				if (lastAppended == lastTag) {
					continue;
				}

				// Create new object in the array
				// Example:
				//     { tag: 2, content: "Some content" }
				node_obj = Object::New();
				node_obj->Set(tag_symbol, Integer::New(lastTag));
				node_obj->Set(data_symbol, String::New(
					analyzer.getSummary(title,
										input.substr(openPos + (lastTag > 1 ? 3 : 2), pos - openPos - (lastTag > 1 ? 3 : 2) - 1),
										sentenceLimit).c_str()));

				nodes->Set(id++, node_obj);
				lastAppended = lastTag;
				continue;
			}

			// Clear useless tags
			input.erase(pos - 1, input.find('>', pos - 1) - pos + 2);
			break;

		// Sets a opening tag in the tag detect
		case Tags::Paragraph:
		case Tags::Heading:
			openPos = pos;
			lastTag = tagID;
			break;
		default:

			// Clear useless tags
			input.erase(pos - 1, input.find('>', pos - 1) - pos + 2);
			break;
		}
	}

	// Remove last element from array if its heading elements
	if (lastTag == Tags::Heading) {
		nodes->Delete(id - 1);
	}

	return nodes;
}