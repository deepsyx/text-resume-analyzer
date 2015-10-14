# text-resume-analyzer

Naive text summarization algorithm in C++11 with wrapper for NodeJS. It was used in Smartly project and won first place in various IT competitions and olympiads.

### Version
0.1.0

### Installation

In order to compile you need to have `NodeJS` and `npm` installed.
You'll also need a C++ compiler, I was using the compiler integrated into Visual studio 2013. To build the module, go into it's directory and execute `npm build .`

### Usage

Include in your NodeJS application with 

    var SmartlyParser = require('smartparser');
    
    var char_limit = 35 * 5; // chars limit
    var title = 'Black mamba'; // text title, has double weight
    var text = 'Big text for black mamba here'; // full text to be sumarized
    
    SmartlyParser.parse('Huge text', 'Text title', char_limit, function(err, result) {
        console.log(result);
    });
    
### Why naive algorithm works. (Wikipedia explanation)

Paragraphs are logical atomic unit of the text and the is a reason why the author decided to split his text that way. The second (and maybe less obvious..) reason is that if two sentences have a good intersection, they probably holds the same information. So if one sentence has a good intersection with many other sentences, it probably holds some information from each one of them- or in other words, this is probably a key sentence in our text!