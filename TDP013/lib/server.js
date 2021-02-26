const http = require('http');
const router = require('./route');
const express = require('express');
const app = express();

function run() {
	app.use('/', router);

	app.use((req, res, next) => {
		res.header("Access-Control-Allow-Origin", "*");
		res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
		next();
	});

	app.use('/jquery', express.static(__dirname + '/node_modules/jquery/dist/'));
	// app.use(express.static('statics'));

	http.createServer(app).listen(8888);
	console.log("Server has started.");
}

exports.run = run; //for running
