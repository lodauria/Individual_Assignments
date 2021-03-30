var mqtt = require('mqtt');
const fs = require('fs');

var mqtthost = 'a39rur5t268yrw-ats.iot.us-east-1.amazonaws.com';
var KEY = fs.readFileSync('../AWS/mydevice.private.key');
var CERT = fs.readFileSync('../AWS/mydevice.cert.pem');
var CAfile = [fs.readFileSync('../AWS/root-CA.crt')];

var options={
	host:mqtthost,
	port: 8883,
	protocol:'mqtts',
	//protocolId: 'MQIsdp',
	ca: CAfile,
	key: KEY,
	cer: CERT,
	secureProtocol: 'TLSv1_method',
	//protocolId: 'MQIsdp',
	protocolVersion: 3
};

var client  = mqtt.connect("a39rur5t268yrw-ats.iot.us-east-1.amazonaws.com:8883",options);

//handle incoming messages
client.on('message',function(topic, message, packet){
	console.log("message is "+ message);
	console.log("topic is "+ topic);
});

// handle connection
client.on("connect",function(){	
console.log("connected  "+ client.connected);
})

//handle errors
client.on("error",function(error){
console.log("Can't connect" + error);
process.exit(1)});

//publish
function publish(topic,msg,options){
console.log("publishing",msg);
}