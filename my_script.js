var debug_status = 0;

function callAwsLambdaFunction(mess_type, mess_debug) {

    $.ajax({
        url: "https://kev4epi1y9.execute-api.us-east-1.amazonaws.com/default/dashboard_handle",
        type: 'POST',
        data: "Request",
        headers: {
        "Access-Control-Allow-Methods": "*",
        "Access-Control-Allow-Headers": "*",
        "Access-Control-Allow-Origin": "*",
		"mess-type": mess_type,
		"mess-debug": mess_debug
    	},
        dataType: 'json',
        crossDomain: true,
        contentType: 'application/json',
        success: function(data){
			console.log("Data received!");
			var myData = JSON.parse(JSON.stringify(data));
			if (typeof myData.proj_last !== 'undefined'){

				document.getElementById("proj_last").innerHTML = "Projector: " + myData.proj_last;
				document.getElementById("light_last").innerHTML = "Lights: " + myData.light_last;

				document.getElementById("proj_avg").innerHTML = "Projector avarage value: " + myData.proj_avg;
				document.getElementById("light_avg").innerHTML = "Light avarage value: " + myData.light_avg;

				document.getElementById("proj_max").innerHTML = "Projector max value: " + myData.proj_max;
				document.getElementById("light_max").innerHTML = "Light max value: " + myData.light_max;

				document.getElementById("proj_min").innerHTML = "Projector min value: " + myData.proj_min;
				document.getElementById("light_min").innerHTML = "Light min value: " + myData.light_min;

				document.getElementById("proj_hist").innerHTML = "Projector: " + myData.proj_hist;
				document.getElementById("light_hist").innerHTML = "Lights: " + myData.light_hist;

				if (myData.act > 0){
					confirm("Actuation done!");
				}
			} else{

				var result = confirm("No data in the last hour. Activate demo mode?");
				if (result == true) {
					debug_status = 1;
					callAwsLambdaFunction(0,debug_status);
				}
			}
        },
        error: function(data) {
		   	console.log("Error receiving data")
        }
    });
}

function toggleRelay() {
	callAwsLambdaFunction(1,debug_status)
}

function toggleLight() {
	callAwsLambdaFunction(2,debug_status)
}

window.onload = callAwsLambdaFunction(0,0);