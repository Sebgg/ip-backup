const mongo = require('mongodb').MongoClient;
const ObjectID = require('mongodb').ObjectID;
const mongoUrl = 'mongodb://localhost:27017/test';
const urlParser = require('url');
const database = 'test';


function paramHandler(req, res, param){
  var ret = urlParser.parse(req.url, true).query[param];

  if (ret === undefined) res.sendStatus(400);
  else return ret.trim();
}

function flag(req, res){
  var id = paramHandler(req, res, 'id');
  mongo.connect(mongoUrl,
      {useNewUrlParser: true,
    	useUnifiedTopology: true},
    	function(err, client) {
        db = client.db(database);
        if (!err) {
          if(!ObjectID.isValid(id))
          {
            res.sendStatus(400);
          } else {
            db.collection('test').findOne({_id: new ObjectID(id)}, (err, doc) =>{
              if(err) res.status(400).send('Id does not exist');
            });
            try {
              db.collection(database).updateOne({"_id": new ObjectID(id)}, {$set :{'flag': true}});
              res.sendStatus(200);
              client.close();
            } catch(error){
              res.sendStatus(500);
              client.close();
            }
          }
        } else res.status(500).send(err);
  });
}

function save(req, res){
  mongo.connect(mongoUrl,
      {useNewUrlParser: true,
      useUnifiedTopology: true},
      function(err, client) {
        db = client.db(database);
        if (!err){
          var msg = paramHandler(req, res, 'msg'); // get message from request
          if(msg === undefined) return; //handles /save without ?msg=
          if (typeof msg != 'string' || msg.length > 140 || msg.length === 0){
            res.status(400).send("Faulty message.").end();
            client.close();
          }else{
            db.collection(database).insertOne({'message': msg, 'flag': false}, function(err, result) {
              if (err) res.status(500).send(err).end();
            });
            db.collection('test').findOne({'message': msg}, (err, doc) =>{
              res.status(200).send({id: doc._id}).end();
            });
            client.close();
          }
        }
        else res.status(500).send(err).end();
    });
}

function getAll(req, res){
  mongo.connect(mongoUrl,
      {useNewUrlParser: true,
      useUnifiedTopology: true},
      function(err, client) {
        db = client.db(database);
        if (!err){
          db.collection(database).find().toArray(function(err, result) {
            if (err) res.sendStatus(500).send(err);
            else res.status(200).json(result);
            db.close;
        });
      } else res.status(500).send(err); //needs testing
      });
}

exports.flag = flag;
exports.save = save;
exports.getAll = getAll;
