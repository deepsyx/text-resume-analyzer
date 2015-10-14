#include <node.h>
#include <string>

using namespace v8;

enum Tags;

// Property names in the output array
// Example:
//   {tag: 2, content: "Content" }
const static Persistent<String> data_symbol = NODE_PSYMBOL("data");
const static Persistent<String> tag_symbol = NODE_PSYMBOL("tag");

// Min text length for paragraphs to be included 
const int MIN_TEXT_LENGTH = 10;

// Accepts char of string and returns the corresponding opening/closing tag
int tagDetect(std::string const& ptr);

// Parses xHTML string without nested tags to object with data and content properties
Local<Array> Parser(std::string & input, std::string const& title, int sentenceLimit, bool analyzerFlag);