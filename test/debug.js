var smartparser = require('../build/Release/smartparser');
var fs = require("fs");

var data = fs.readFileSync('data.txt').toString();

smartparser.parse(data, "Black mamba", 5 * 36, function(err, result) {
    console.warn(result);
});