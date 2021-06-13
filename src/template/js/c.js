console.log("./js/c.js:start");
let lib=require(1,"lib.js");
let i=0;
while(i<8){
	lib.f0();
	lib.f1("hi");
	i++;
}
console.log("./js/c.js:end");
