RVM.loadModule("KoalaCURL");

CURL.init();

var curl = new CURL();
curl.setURL(_args[0]);
var res = curl.perform();
console.log(res);

CURL.quit();
