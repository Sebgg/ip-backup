const express = require('express');
const router = express.Router();
let messageHandler = require('./handlers/messageHandler');
let authHandler = require('./handlers/authHandler');
let userHandler = require('./handlers/userHandler');
let tokenHandler = require('./handlers/tokenHandler');
let checkToken = tokenHandler.checkToken;
const database = 'test';
const path = require('path');


router.use(function timeLog(req, res, next){
  console.log('Time: ', Date.now());
  next();
});

router.get('/', function(req, res){
  res.sendStatus(200);
});

//---Message-related-routes-----------------------------------------------------
router.post('/save', checkToken, function(req, res) {
  messageHandler.save(req, res);
});

router.post('/getmessagesuser', checkToken, async function(req, res){
  messageHandler.getMessagesUser(req, res);
}); // for production

// router.post('/save', function(req, res) {
//   messageHandler.save(req, res);
// });
//
// router.post('/getmessagesuser', function(req, res){
//   messageHandler.getMessagesUser(req, res);
// }); // for testing

//------------------------------------------------------------------------------
//---User-related-routes--------------------------------------------------------
router.post('/auth/login', function(req, res){
  authHandler.login(req, res);
});

router.post('/auth/signup', function(req, res){
  authHandler.signup(req, res);
});

router.post('/user/getinfo', checkToken, function(req, res){
  userHandler.getUserInfo(req, res);
}); // for production

// router.post('/user/getinfo', function(req, res){
//   userHandler.getUserInfo(req, res);
// }); // for testing

router.post('/user/finduser', function(req, res) {
  userHandler.findUser(req, res);
});

router.post('/user/addfriend', function(req, res) {
  userHandler.addFriend(req, res);
});
//------------------------------------------------------------------------------
//----Remove-token-when-logged-out----------------------------------------------
router.post('/removetoken', function(req, res, next) {
  tokenHandler.removeToken(req, res, next);
});
//------------------------------------------------------------------------------
//---Faulty-get-and-post-methods------------------------------------------------
router.get('*', function(req, res){
  res.sendStatus(404);
});

router.post('/', function(req, res){
  res.sendStatus(405);
});
//------------------------------------------------------------------------------
module.exports = router;
