const http = require('http'),
 			router = require('./route'),
			express = require('express'),
			cors = require('cors'),
			mongoose = require('mongoose'),
			bodyParser = require('body-parser'),
      cookieParser = require('cookie-parser');

// create app
const app = express();

// include config-file
const config = require("./config")

// CORS options
var corsOptions = {
  credentials: true,
  origin: 'http://localhost:3000'
}

// app uses cookieparser for authentication middleware
app.use(cookieParser());

// app uses cors plugin for CORS
app.use(cors(corsOptions));

// app uses bodyParser
app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());


function run() {
	// app uses router for easier routing
	app.use('/', router);

	// mongoose connections
	mongoose.connect(config.MONGO_URI, {useNewUrlParser: true,
																			useUnifiedTopology: true,
																			useCreateIndex: true})
		.then(() => console.log('Connected to MongoDB!'))
		.catch((err) => console.log('Something went wrong:',err));
	let db = mongoose.connection;
	db.on('error', function(err) {
  	console.log('Error: Could not connect to MongoDB.');
	});

	http.createServer(app).listen(config.LISTEN_PORT);
	console.log("Server has started on " + config.LISTEN_PORT);
}

exports.run = run;
