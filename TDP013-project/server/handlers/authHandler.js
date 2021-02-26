let jwt = require('jsonwebtoken');
const config = require('../config');
let Users = require('../models/user.model');

exports.login = (req, res) => {
  let email = req.body.email,
      password = req.body.passw;

  var correctUserCred = false; // true if pass matches username's pass in db
  // For the given username fetch user from DB
  Users.findOne({email: email}, function(err, user) {
    if (!err && user) {
      if (email && password) {
        correctUserCred = user.comparePassword(password); // async is bitch
        if (correctUserCred) {
          // return the JWT token for the future API calls
          let token = jwt.sign({email: email}, config.TOKEN_SECRET);
          res.status(200).cookie('token', token, {httpOnly: true, path: '/'});
          res.json({
            success: true,
            message: 'Authentication successful!',
            email: email
          });
        } else {
          res.status(401).json({
            success: false,
            message: 'Incorrect password'
          });
        }
      } else {
        res.status(401).json({
          success: false,
          message: 'Authentication failed! Please check the request'
        });
      }
    } else if (err || !res) {
      res.status(500).json({
        success: false,
        message: 'User not found'
      });
    }
  });
}

exports.signup = (req, res) => {
  let uname = req.body.uname,
      email = req.body.email,
      passw = req.body.passw;

  let data = new Users({
    username: uname,
    email: email,
    password: passw
  });

  data.save(function(err, user) {
    if (!err) {
      let token = jwt.sign({email: email}, config.TOKEN_SECRET);
      res.status(200).cookie('token', token, {httpOnly: true, path: '/'});
      res.json({
        success: true,
        message: 'Authentication successful!',
        email: email
      });
    }
    else res.status(500).send(err);
  });
}
