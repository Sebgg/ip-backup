const config = require('../config');
let Users = require('../models/user.model');

exports.addFriend = (req, res) => {
  let user = req.body.email,
      friend = req.body.femail;


  Users.findOne({email: user}, function(err, fUser) {
    if (!err && fUser) {
      Users.findOne({email: friend}, function(err, fFriend) {
        if (!err && fFriend && !fUser.friends.includes(fFriend.email)) {
          fUser.friends.push(fFriend.email);
          fFriend.friends.push(fUser.email);
          fUser.save();
          fFriend.save();
          res.sendStatus(204);
        } else res.status(500).send('FriendNotFound');
      });
    } else res.status(500).send('UserNotFound');
  });
}

exports.getUserInfo = (req, res) => {
  let user;

  if ('fEmail' in req.body) {
    user = req.body.fEmail;
  } else {
    user = req.body.email;
  }

  Users.findOne({email: user}, function(err, found) {
    if (!err && found) {
      res.status(200).json(found);
    } else res.status(500).send(err);
  });
}


exports.findUser = (req, res) => {
  let user = req.body.query;

  Users.findOne({email: user}, function(err, eFound) {
    if (!err && eFound) {
      res.status(200).json(eFound);
    } else if (!eFound){
      Users.find({username: user}, function(err, uFound) {
        if (!err && uFound) {
          res.status(200).json(uFound);
        } else if (!uFound){
          res.status(400).send('NoUserFound');
        } else {
          res.status(500).send(err);
        }
      });
    } else {
      res.status(500).send(err);
    }
  });
}
