// Global variable used for demo mode
var debug_status = 0;
var dev_id = 0;

function callAwsLambdaFunction(mess_type, mess_debug, device_id) {

	// Send HTTP POST message to the API gateway using Ajax
    $.ajax({

    	// HTTP API gateway URL
        url: "https://kev4epi1y9.execute-api.us-east-1.amazonaws.com/default/dashboard_handle",
        type: 'POST',
        data: "Request",
        dataType: 'json',
        crossDomain: true,
        contentType: 'application/json',

        // Use custom headers to specify the type of request
        headers: {
	        "Access-Control-Allow-Methods": "*",
	        "Access-Control-Allow-Headers": "*",
	        "Access-Control-Allow-Origin": "*",
			"mess-type": mess_type,
			"mess-debug": mess_debug,
			"dev-id": device_id
    	},

        // If response is received correctly 
        success: function(data) {

        	// Parse message and check if format is correct
			console.log("Data received!");
			var myData = JSON.parse(JSON.stringify(data));

			if (typeof myData.proj_last !== 'undefined'){

				// Update HTML page text
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

				// In case it was executed, confirm actuation command execution
				if (myData.act > 0){
					confirm("Actuation done!");
				}

			} else{

				// If parse was wrong probably is because there aren't recent data in the DynamoDB
				var result = confirm("No data in the last hour. Activate demo mode?");

				// Ask to switch to demo mode
				if (result == true) {
					debug_status = 1;
					callAwsLambdaFunction(0,debug_status);
				}
			}
        },

        // In case of error report failure
        error: function(data) {
		   	console.log("Error receiving data");
        }
    });
}

// Call lambda function with specific parameters
function toggleRelay() {
	callAwsLambdaFunction(1,debug_status,dev_id)
}

// Call lambda function with specific parameters
function toggleMotor() {
	callAwsLambdaFunction(2,debug_status,dev_id)
}

function updateDev() {
  dev_id = document.getElementById('id_dev').value;
  callAwsLambdaFunction(0,debug_status,dev_id)
}

// Call lambda function the first time to initialize the page
window.onload = callAwsLambdaFunction(0,0,0);