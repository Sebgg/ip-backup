const config = require('../config'),
      ObjectID = require('mongoose').Schema.Types.ObjectID;

let Users = require('../models/user.model'),
    Messages =  require('../models/message.model');

// Saves a message to the message collection
exports.save = (req, res) => {

  if (req.body.email) {
    author = req.body.email;
  } else {
    res.status(401).send('NotLoggedIn');
    return;
  }

  let owner = req.body.owner,
      input = req.body.message;

  let message = new Messages({
    author: author,
    owner: owner,
    message: input
  });

  message.save(function(err, post) {
    if (!err) res.sendStatus(201);
    else res.status(500).send(err); // can't test
  });
}

//getall for requesting user
exports.getMessagesUser = (req, res) => {
  var owner = null;
  if ('fEmail' in req.body) {
    owner = req.body.fEmail;
  } else if ('email' in req.body){
    owner = req.body.email;
  } else {
    res.status(401).send('NoUserSpecified');
    return;
  }
  var sendBack = [];

  // is tested, even though istanbul doesn't think so...
  Messages.find({owner: owner}, function(err, docs) {
    if (!err && docs) {
      docs = docs.reverse();
      res.status(200).json(docs);
    } else res.sendStatus(500); // can't test
  }).lean();
}
