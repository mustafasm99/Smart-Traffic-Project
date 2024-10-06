let IP              = null
let interval        = null
let apikey          = null 



function setIP(ele){
    IP          = ele.value
}

function setAPI(ele){
    apikey   = ele.value
}

function get_data(){
    let url = `http://${IP}/get_info`
    fetch(url)
    .then(res=>{
        if (!res.ok){
            throw new Error('network error' + response.statusText)
        }
        return res.json()
    })
    .then(data=>{
        console.log(data);
        setData(data)
    })
}

function send_request(streetNumber){

    if(apikey == null ){alert("Enter your API key please .. !")}

    fetch(`http://${IP}/setmode?mode=2&roudNumber=${streetNumber}&apikey=${apikey}`).then(res=>{
        if(res.status === 200){
            alert('requset send to the '+streetNumber)
            return res.text();
        }else if (res.status == 401){
            alert("your api key is wrong ... ? ");
        }else {
            alert('An error occurred: ' + res.status);
        }
    })
}


function start(){
    if(interval != null){clearInterval()}
    interval =  setInterval(get_data , 3200)
}

function setData(data){
    if(data){
        let carsNumbers     = document.querySelectorAll("#cars-number")
        let trafficLights   = document.querySelectorAll("#traffic-light")
    
        for(var i=0; i<4; i++){
            carsNumbers[i].innerText    = data.CarCount[i]
            trafficLights[i].innerText  = data.TrafficStates[i]
        }
    }
}

