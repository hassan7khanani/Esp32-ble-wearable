import {DBSCAN, clusterReduce, findEpisodes} from "./DBSCAN.js";
import { highpass } from "./filter.js";
import _ from "lodash"

/*testing

function randomDate(start, end) {
    start = Date.parse(start);
    end = Date.parse(end);

    return new Date(Math.floor(Math.random() * (end - start + 1) + start));
}

*/

const zeroArray = [];
const halfsec = 500;

function pair(pressure, time){
    //let date = new Date(time)
    return {
        gr: pressure,
        ts: new Date (time)
    };
};

//testing new
//change to date objects


//Test 1: should create 4 new arrays before last index
console.log("TEST 1: \n")
var dataArray1 = [pair([1],1651535597580), pair([1],1651535597585),pair([1],1651535597590),pair([1],1651535599594)];
console.log(analyze(dataArray1, 90));
console.log("\n")
//Test 2: Should create no new arrays
console.log("TEST 2: \n")
var dataArray2 = [pair([0],1651535597580), pair([1,2],1651535597580)];
console.log(analyze(dataArray2, 90));
console.log("\n")
//Test 3: Should create 1 array after index 0
console.log("TEST 3: \n")
var dataArray3 = [pair([1,2],1651535599999), pair([1,2],1651535600500),pair([1,2],1651535600510),pair([1,2],1651535600511)];
console.log(analyze(dataArray3, 90));
console.log("\n")
//Test 4: Should create an array between each point
console.log("TEST 4: \n")
var dataArray4 = [pair([1,2],1651535600500), pair([1,2],1651535601001),pair([1,2],1651535601502),pair([1,2],1651535602003)];
console.log(analyze(dataArray4, 90));
console.log("\n")
//Test 5: Should create 2 arrays between each point
console.log("TEST 5: \n")
var dataArray5 = [pair([1,2],1651535600500), pair([1,2],1651535601502),pair([1,2],1651535602504),pair([1,2],1651535603507)];
console.log(analyze(dataArray5, 90));
console.log("\n")




// .5 seconds


//given function

export function analyze(arr, noiseThreshold){
    console.log("analyze reached")
    return findEpisodes(fillZero(arr, noiseThreshold))
}

export function fillZero(arr, noiseThreshold){
    console.log("here")
    var fixedArr = [];
    //grab first array
    let prevtime = arr[0].ts;
    let currtime = arr[1].ts

    //convert in milliseconds
    prevtime = prevtime.getTime();
    currtime = currtime.getTime();
    //create zero array
    
    // get the length of the  gr array
    var zeroLen = arr[0].gr.length;

    //create arrays according to the length of zeroLen 
    var zeroArray = createArray(zeroLen);
    var oneArray = createArray(zeroLen);

    //fill 1 to each location
    oneArray.fill(1)

    //loop through array
    //"gr": [{"sensor1": 0,"sensor2": 111}],prevtime


    fixedArr.push(pair(arr[0].gr,new Date(prevtime)))
    
    for(let i = 1; i < arr.length; i++) {
         prevtime = arr[i-1].ts;
         currtime = arr[i].ts;
        prevtime = prevtime.getTime();
        currtime = currtime.getTime();
        const diff = currtime - prevtime;


        const arraysToAdd = Math.trunc(diff/halfsec);
        // returns integar value
        //if there is more than a half second of space
        
        if (diff >  halfsec){
           
        //add a zero array at every half second since prevtime
            var temp = prevtime+ halfsec;
           // var date = new Date (temp);
            for (let j = 0; j < arraysToAdd; j++){
                fixedArr.push ({
                    gr: zeroArray,
                    ts: new Date (temp)
                })
                temp = temp + halfsec;
           }
        }
        fixedArr.push(pair(arr[i].gr,new Date (currtime)))
    }


    let shavedArr = []
    biArr = highpass(fixedArr, zeroArray)
    for(let i = 0; i<biArr.length; i++){
        if(isPoint(biArr[i].gr)){
            shavedArr.push(_.cloneDeep(biArr[i]))
        }
    }

    for(let i = 0; i < biArr.length; i++){
        if(!isPoint(biArr[i].gr)){
            biArr[i].gr = zeroArray
        }
        else{
            biArr[i].gr = oneArray
        }
    }
   // console.log(biArr)


    return [biArr,shavedArr];
    
}

function isPoint(gr){
    for(let i = 0; i<gr.length; i++){
        if(gr[i] != 0){
            return true
        }
    }
    return false
}

export function fillZeroRaw(arr){
    var fixedArr = [];
    //grab first array
    var prevtime = new Date(arr[0].ts);
    var currtime = new Date (arr[1].ts);
    console.log(prevtime)
    prevtime = prevtime.getTime();
    currtime = currtime.getTime();
    //create zero array
    var zeroLen = arr[0].gr.length;
    var zeroArray = createArray(zeroLen);
    var oneArray = createArray(zeroLen);
    oneArray.fill(1)
    //loop through array
    fixedArr.push(pair(arr[0].gr,new Date(prevtime)))
    for(let i = 1; i < arr.length; i++) {
         prevtime = new Date(arr[i-1].ts);
         currtime = new Date(arr[i].ts);
        prevtime = prevtime.getTime();
        currtime = currtime.getTime();
        const diff = currtime - prevtime;
        const arraysToAdd = Math.trunc(diff/halfsec);
        //if there is more than a half second of space
        
        if (diff >  halfsec){
           
        //add a zero array at every half second since prevtime
            var temp = prevtime+ halfsec;
           // var date = new Date (temp);
            for (let j = 0; j < arraysToAdd; j++){
                fixedArr.push ({
                    gr: zeroArray,
                    ts: new Date (temp)
                })
                temp = temp + halfsec;
           }
        }
        fixedArr.push(pair(arr[i].gr,new Date (currtime)))
    }
    //console.log(fixedArr)
    return fixedArr;
    
}

//replace with array of given set size
//make obj




//create zeroArray
function createArray(zeroLen){
    let zeroArray = [];
    for (var i = 0; i < zeroLen; i++) {
        zeroArray.push(0);
    }
    return zeroArray;

}


