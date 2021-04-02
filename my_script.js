function callAwsLambdaFunction(type) {

	var mess = "{ \"type\": ";
	mess = mess.concat(type, "}");
    $.ajax({
        url: "https://gcgt6w6uze.execute-api.us-east-1.amazonaws.com/default/merda",
        type: 'POST',
        data: mess,
        headers: {
        "Access-Control-Allow-Methods": "*",
        "Access-Control-Allow-Headers": "*",
        "Access-Control-Allow-Origin": "*"
    	},
        dataType: 'json',
        crossDomain: true,
        contentType: 'application/json',
        success: function(data){
        	console.log("New data received");
		   	var myData = JSON.parse(JSON.stringify(data));

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
        },
        error: function(e) {
            console.log("Error receiving data:\n" + JSON.stringify(e));
        }
    });
}

window.onload = callAwsLambdaFunction(0);