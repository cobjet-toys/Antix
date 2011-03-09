var fs = require('fs');

var arr = new Array();

var data = {"id" : "1", "position" : "10", "speed" : "100"};	

for (i=0; i < 10000; i++){

   arr[i] = data;

   //console.log(arr[i]);

}

//console.log(arr)

var jsonStr = JSON.stringify(arr);

fs.writeFile('message2.txt', jsonStr, function (err) {
  if (err) throw err;
  console.log('It\'s saved!');
});
