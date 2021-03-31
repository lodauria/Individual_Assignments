function callAwsLambdaFunction(type) {

	var mess = "{ \"type\": ";
	mess = mess.concat(type, "}");
    $.ajax({
        url: "https://354b6u6uv8.execute-api.us-east-1.amazonaws.com/default/dashboard_handle",
        type: 'POST',
        data: mess,
        dataType: 'json',
        crossDomain: true,
        contentType: 'application/json',
        success: mess_handle,
        error: function(e) {
            console.log("Error receiving data:\n" + JSON.stringify(e));
        }
    });
}

var mess_handle = function (data) {
	console.log("New data received");
   	var myData = JSON.parse(JSON.stringify(data));

    document.getElementById("proj_last").innerHTML += myData.proj_last;
    document.getElementById("light_last").innerHTML += myData.light_last;

    document.getElementById("proj_avg").innerHTML += myData.proj_avg;
    document.getElementById("light_avg").innerHTML += myData.light_avg;

    document.getElementById("proj_max").innerHTML += myData.proj_max;
    document.getElementById("light_max").innerHTML += myData.light_max;

    document.getElementById("proj_min").innerHTML += myData.proj_min;
    document.getElementById("light_min").innerHTML += myData.light_min;

    document.getElementById("proj_hist").innerHTML += myData.proj_hist;
    document.getElementById("light_hist").innerHTML += myData.light_hist;

    if (myData.act > 0){
    	confirm("Actuation done!");
    }

};

window.onload = callAwsLambdaFunction(0);