const mongo = require('mongodb').MongoClient;
const objectID = require('mongodb').ObjectID;
const request = require('superagent');
var expect = require('chai').expect;
const app = require('../lib/main.js');
const serverUrl = '0.0.0.0:8888'
const api = request(app);
var flagId = null;
//--------Test-messages---------------------------------------------------------
const testMessageOne = 'this is a testmessage';
const testMessageTwo = 'this is also a testmessage';
const emptyTestMessage = '';
const toLongTestMessage = 'this is not supposed to be added to database because it is more than 140 characters long. it is really hard to come up with text to write, so this is just random bullshit'
//------------------------------------------------------------------------------
//--------Mongo-setup--------------------------------------------------------
const mongoUrl = 'mongodb://localhost:27017/test'
//------------------------------------------------------------------------------
//--------Clean-up-after-test---------------------------------------------------
after((done) => {
  mongo.connect(mongoUrl,
  {useNewUrlParser: true,
    useUnifiedTopology: true},
    function(err, client) {
      if (err) throw err;
      db = client.db('test')
      db.collection('test').deleteMany({});
      client.close();
      done();
  });
});
//------------------------------------------------------------------------------

describe('Get startpage', () => {
  describe('Get /', () => {
    it('returns statuscode 200', (done) => {
      request.get(serverUrl, function(err, res){
        expect(res.statusCode).to.equal(200);
        done();
      });
    });

    it('does not return an error', (done) => {
      request.get(serverUrl, function(err, res){
        expect(err).to.be.null;
        done();
      });
    });
  });
});

describe('Get 404-error', () => {
  describe('Get /set', () => {
    it('returns statuscode 404', (done) => {
      request.get(serverUrl + '/set', function(err, res){
        expect(res.statusCode).to.equal(404);
        done();
      });
    });
  });
});

describe('Get 405-error', () => {
  describe('Post /', () => {
    it('returns statuscode 405', (done) => {
      request.post(serverUrl + '/', function(err, res){
        expect(res.statusCode).to.equal(405);
        done();
      });
    });
  });
});

describe('Get all objects in database', () => {
  describe('Get /getall', () => {
    it('returns statuscode 200', (done) => {
      request.get(serverUrl + '/getall', function(err, res){
        expect(res.statusCode).to.equal(200);
        done();
      });
    });

    it('returns a JSON object', (done) => {
      request(serverUrl, function(err, res){
        expect(res.body).to.be.an('object');
        done();
      });
    });

    it('should have 0 objects in res and in db', (done) => {
      request.get(serverUrl + '/getall', function(err, res){
        expect(Object.keys(res.body).length).to.equal(0);
        //-----Check-if-database-changed-in-size--------------------------------
        mongo.connect(mongoUrl,
          {useNewUrlParser: true,
            useUnifiedTopology: true},
            function(err, client) {
              if (err) throw err;
              db = client.db('test')
              db.collection('test').countDocuments({}, (err, numOf) => {
                expect(numOf).to.equal(0);
                client.close();
                done();
              });
            });
        //----------------------------------------------------------------------
      });
    });

    it('should not return an error', (done) => {
      request.get(serverUrl + '/getall', function(err, res){
        expect(err).to.be.null;
        done();
      });
    });
  });
});

describe('Save objects to database', () => {
  describe('Check that db is empty before save', () => {
      it('should be empty when countDocuments is called', (done) => {
        //-----Check-that-database-size-=-zero----------------------------------
        mongo.connect(mongoUrl,
          {useNewUrlParser: true,
            useUnifiedTopology: true},
            function(err, client) {
              if (err) throw err;
              db = client.db('test')
              db.collection('test').countDocuments({}, (err, numOf) => {
                expect(numOf).to.equal(0);
                client.close();
                done();
              });
            });
        //----------------------------------------------------------------------
      });
  });

  describe('Get /save with message', () => {
    it('returns statuscode 200 and no errors', (done) => {
      request.get(serverUrl + '/save?msg=' + testMessageOne, function(err, res){
        expect(res.statusCode).to.equal(200);
        expect(err).to.be.null;
        //-----Check-if-database-changed-in-size--------------------------------
        mongo.connect(mongoUrl,
          {useNewUrlParser: true,
            useUnifiedTopology: true},
            function(err, client) {
              if (err) throw err;
              db = client.db('test')
              db.collection('test').countDocuments({}, (err, numOf) => {
                expect(numOf).to.equal(1);
                client.close();
                done();
              });
            });
        //----------------------------------------------------------------------
      });
    });

    it('res body-size should be 1', (done) => {
      request.get(serverUrl + '/getall', function(err, res) {
        expect(Object.keys(res.body).length).to.equal(1);
        done();
      });
    });

    it('should not save a empty message', (done) => {
      request.get(serverUrl + '/save?msg=' + emptyTestMessage, (err, res) => {
        expect(res.statusCode).to.equal(400);
        //-----Check-if-database-changed-in-size--------------------------------
        mongo.connect(mongoUrl,
          {useNewUrlParser: true,
            useUnifiedTopology: true},
            function(err, client) {
              if (err) throw err;
              db = client.db('test')
              db.collection('test').countDocuments({}, (err, numOf) => {
                expect(numOf).to.equal(1);
                client.close();
                done();
              });
            });
        //----------------------------------------------------------------------
      });
    });

    it('should not save a message over 140 chars', (done) => {
      request.get(serverUrl + '/save?msg=' + toLongTestMessage, (err, res) => {
        expect(res.statusCode).to.equal(400);
        //-----Check-if-database-changed-in-size--------------------------------
        mongo.connect(mongoUrl,
          {useNewUrlParser: true,
            useUnifiedTopology: true},
            function(err, client) {
              if (err) throw err;
              db = client.db('test')
              db.collection('test').countDocuments({}, (err, numOf) => {
                expect(numOf).to.equal(1);
                client.close();
                done();
              });
            });
        //----------------------------------------------------------------------
      });
    });

    it('should throw 400 when no ?msg', (done) => {
      request.get(serverUrl + '/save', function(err, res) {
        expect(res.statusCode).to.equal(400);
        done();
      });
    });
  });
});

describe('Flag a object in database', () => {
  describe('Save object to database', () => {
    it('returns statuscode 200 and no errors', (done) => {
      request.get(serverUrl + '/save?msg=' + testMessageTwo, function(err, res){
        expect(res.statusCode).to.equal(200);
        expect(err).to.be.null;
        done();
      });
    });

    it('should have two items in collection', (done) =>{
      request.get(serverUrl + '/getall', function(err, res) {
        expect(Object.keys(res.body).length).to.equal(2);
        //-----Check-if-database-changed-in-size--------------------------------
        mongo.connect(mongoUrl,
          {useNewUrlParser: true,
            useUnifiedTopology: true},
            function(err, client) {
              if (err) throw err;
              db = client.db('test')
              db.collection('test').countDocuments({}, (err, numOf) => {
                expect(numOf).to.equal(2);
              });

              db.collection('test').findOne({'message': testMessageTwo}, (err, doc) =>{
                flagId = doc._id; //Get id of new message from db for next test
                client.close();
                done();
              });
            });
        //----------------------------------------------------------------------
      });
    });

    it('should be able to flag a message', (done) => {
      request.get(serverUrl + '/flag?id=' + flagId.toString(), (err, res) => {
        expect(res.statusCode).to.equal(200);
        //-----Check-flag-is-true-in-db-----------------------------------------
        mongo.connect(mongoUrl,
          {useNewUrlParser: true,
            useUnifiedTopology: true},
            function(err, client) {
              if (err) throw err;
              db = client.db('test')
              db.collection('test').findOne({_id: new objectID(flagId)}, (err, res) => {
                if(err) throw err;
                expect(res.flag).to.be.true;
                client.close();
                done();
            });
        });
        //----------------------------------------------------------------------
      });
    });

    it('should throw 400-error when invalid id', (done) => {
      request.get(serverUrl + '/flag?id=1234', (err, res) => {
        expect(res.statusCode).to.equal(400);
        done();
      });
    });
  });
});
