const express = require('express');
const router = express.Router();
const dataHandler = require('./databaseHandler')
const database = 'test';
const path = require('path');

router.use(function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});

router.use(function timeLog(req, res, next){
  console.log('Time: ', Date.now());
  next();
});

router.get('/', function(req, res){
  res.sendStatus(200);
});


router.get('/flag', async function(req, res) {
  dataHandler.flag(req, res);
});

router.get('/save', async function(req, res) {
  dataHandler.save(req, res);
});

router.get('/getall', async function(req, res){
  dataHandler.getAll(req, res);
});

router.get('*', function(req, res){
  res.sendStatus(404);
});

router.post('/', function(req, res){
  res.sendStatus(405);
});

module.exports = router;
