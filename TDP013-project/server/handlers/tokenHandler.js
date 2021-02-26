let jwt = require('jsonwebtoken');
const config = require('../config');
let Users = require('../models/user.model');

exports.checkToken = (req, res, next) => {
  const token =
    req.body.token ||
    req.query.token ||
    req.headers['x-access-token'] ||
    req.cookies.token;

  if (token) {
    jwt.verify(token, config.TOKEN_SECRET, (err, decoded) => {
      if (err) {
        console.log('Invalid token');
        res.status(401).send('Unauthorized: Invalid token');
      } else {
        req.body.email = decoded.email;
        console.log('Correct token');
        next();
      }
    });
  } else {
    res.status(401).send('Unauthorized: No token provided');
  }
}

exports.removeToken = (req, res, next) => {
  res.clearCookie('token');
  next();
}
