function callAwsLambdaFunction() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("myDiv").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "https://test123.ap-south-1.amazonaws.com/dev", true);
    xhttp.send();
}