import _ from "lodash"

const MAX_CONSECUTIVE = 40

export function highpass(arr, zeroArray) {
    let insertedArr = _.cloneDeep(arr)
    let filteredArr = []
    let consecutiveArr = []

    for(let i = 0; i<insertedArr.length; i++){
        //console.log(insertedArr[i].gr)
        if(isPoint(insertedArr[i].gr)){
            consecutiveArr.push(_.cloneDeep(insertedArr[i]))
        }
        else{
            if(consecutiveArr.length<MAX_CONSECUTIVE){
                for(let j = 0; j<consecutiveArr.length; j++){
                    filteredArr.push(_.cloneDeep(consecutiveArr[j]))
                }
            }
            else{
                for(let j = 0; j<consecutiveArr.length; j++){
                    filteredArr.push(
                       { 
                        gr:zeroArray,
                        ts: _.cloneDeep(consecutiveArr[j]).ts
                        }
                    )
                }
            }
            consecutiveArr = []
            filteredArr.push(_.cloneDeep(insertedArr[i]))
        }
    }
    if(consecutiveArr.length<MAX_CONSECUTIVE){
        for(let j = 0; j<consecutiveArr.length; j++){
            filteredArr.push(_.cloneDeep(consecutiveArr[j]))
        }
    }
    else{
        for(let j = 0; j<consecutiveArr.length; j++){
            filteredArr.push(
               { 
                gr:zeroArray,
                ts: _.cloneDeep(consecutiveArr[j]).ts,
                }
            )
        }
    }
    consecutiveArr = []

    //return insertedArr
    console.log("20 sec removed arr")
    console.log(filteredArr)
    return filteredArr
}

function isPoint(gr){
    for(let i = 0; i<gr.length; i++){
        if(gr[i] != 0){
            return true
        }
    }
    return false
}