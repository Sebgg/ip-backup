const request = require('superagent');
var expect = require('chai').expect;
const app = require('../server/main.js'),
      config = require('../server/config.js'),
      serverUrl = '0.0.0.0:' + config.LISTEN_PORT,
      Messages = require('../server/models/message.model'),
      Users = require('../server/models/user.model'),
      api = request(app),
      mongoose = require('mongoose');

//--------Test-messages---------------------------------------------------------
// const testMessageOne = 'this is a testmessage';
// const testMessageTwo = 'this is also a testmessage';
// const emptyTestMessage = '';
// const toLongTestMessage = 'this is not supposed to be added to database because it is more than 140 characters long. it is really hard to come up with text to write, so this is just random bullshit'
//------------------------------------------------------------------------------
//--------Mongoose-setup--------------------------------------------------------
mongoose.connect(config.MONGO_URI, {useNewUrlParser: true,
                                    useUnifiedTopology: true,
                                    useCreateIndex: true})
  .then(() => console.log('Connected to MongoDB!'))
  .catch((err) => console.log('Something went wrong:',err));
let db = mongoose.connection;
db.on('error', function(err) {
  console.log('Error: Could not connect to MongoDB.');
});
//------------------------------------------------------------------------------
//--------Clean-up-dev-db-before-test-------------------------------------------
before((done) => {
  Users.deleteMany({}, function(err) {
    if (err) console.log(err);
  });
  Messages.deleteMany({}, function(err) {
    if (err) console.log(err);
  });
  done();
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

describe('Handling user in database', () => {
  describe('Add user to database', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/auth/signup')
      .send({uname: "dev1", email: "b@gmail.com", passw: "qwe"})
      .then((res) => {
        expect(res.body.success).to.be.true;
        expect(res.statusCode).to.equal(200);
      });
    });
  });

  describe('Logging in to website', () => {
    it('returns statuscode 200 and success message', () => {
      request.post(serverUrl + '/auth/login')
      .send({email: "b@gmail.com", passw: "qwe"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.success).to.be.true;
        expect(res.body).to.include('Authentication successful!');
      });
    });
  });

  describe('Add another user to database', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/auth/signup')
      .send({uname: "dev2", email: "c@gmail.com", passw: "qwe"})
      .then((res) => {
        expect(res.body.success).to.be.true;
        expect(res.statusCode).to.equal(200);
      });
    });
  });

  describe('Make dev1 and dev2 friends', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/user/addfriend')
      .send({femail: "b@gmail.com", email: "c@gmail.com"})
      .then((res) => {
        expect(res.statusCode).to.equal(204);
      });
    });
  });

  describe('Get user info', () => {
    it('returns statuscode 200 and info', () => {
      request.post(serverUrl + '/user/getinfo')
      .send({email: "b@gmail.com"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.length).to.be.gt(0);
      });
    });
  });

  describe('Get friend info', () => {
    it('returns statuscode 200 and info', () => {
      request.post(serverUrl + '/user/getinfo')
      .send({femail: "b@gmail.com"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.length).to.be.gt(0);
      });
    });
  });

  describe('Find a user by email', () => {
    it('returns statuscode 200 and info', () => {
      request.post(serverUrl + '/user/getinfo')
      .send({query: "b@gmail.com"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.length).to.be.gt(0);
      });
    });
  });

  describe('Find a user by username', () => {
    it('returns statuscode 200 and info', () => {
      request.post(serverUrl + '/user/getinfo')
      .send({query: "dev1"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.length).to.be.gt(0);
      });
    });
  });

  describe('Try to find a user that doesn\'t exist', () => {
    it('returns statuscode 400 and info', () => {
      request.post(serverUrl + '/user/getinfo')
      .send({query: "dev7"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(400);
        expect(res.text).to.equal('NoUserFound');
      });
    });
  });

  describe('Try to find add a friend that doesn\'t exist', () => {
    it('returns statuscode 400 and info', () => {
      request.post(serverUrl + '/user/addfriend')
      .send({email: "b@gmail.com", femail:"noexist"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(5090);
        expect(res.text).to.equal('FriendNotFount');
      });
    });
  });
});

describe('Save and operate on message', () => {
  describe('Add user to database', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/auth/signup')
      .send({uname: "test", email: "a@mail.com", passw: "asd"})
      .then((res) => {
        expect(res.statusCode).to.equal(200);
      });
    });
  });

  describe('Add a message to database', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/save')
      .send({email: "b@mail.com", owner: "a@mail.com", message: "test numero uno!"})
      .then((res) => {
        expect(res.statusCode).to.equal(201);
      });
    });
  });

  describe('Add another message to database, same user', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/save')
      .send({email: "b@mail.com", owner: "a@mail.com", message: "test numero dos!"})
      .then((res) => {
        expect(res.statusCode).to.equal(201);
      });
    });
  });

  describe('Add another message to database, same user', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/save')
      .send({email: "b@mail.com", owner: "a@mail.com", message: "test numero tres!"})
      .then((res) => {
        expect(res.statusCode).to.equal(201);
      });
    });
  });

  describe('Add another message to database, same user', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/save')
      .send({email: "b@mail.com", owner: "a@mail.com", message: "test numero quattro!"})
      .then((res) => {
        expect(res.statusCode).to.equal(201);
      });
    });
  });

  describe('get all messages from user', () => {
    it('returns statuscode 200 and res size = 4', () => {
      request.post(serverUrl + '/getmessagesuser')
      .send({email: "a@mail.com"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.length).to.equal(4);
      });
    });
  });

  describe('add message to another user', () => {
    it('returns statuscode 200', () => {
      request.post(serverUrl + '/save')
      .send({email: "a@gmail.com", owner: "b@mail.com", message: "test numero cinco!"})
      .then((res) => {
        expect(res.statusCode).to.equal(201);
      });
    });
  });


  describe('get all messages from user again', () => {
    it('returns statuscode 200 and res size = 4', () => {
      request.post(serverUrl + '/getmessagesuser')
      .send({email: "a@mail.com"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.length).to.equal(4);
      });
    });
  });

  describe('get all messages from friend', () => {
    it('returns statusCode 200 and res size = 1', () => {
      request.post(serverUrl + '/getmessagesuser')
      .send({fEmail: "b@mail.com"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(200);
        expect(res.body.length).to.equal(1);
      });
    });
  });

  describe('save a message without email in body', () => {
    it('returns statusCode 401 and NotLoggedIn', () => {
      request.post(serverUrl + '/save')
      .send({owner: "b@mail.com", message: "401 testing"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(401);
        expect(res.text).to.equal('NotLoggedIn');
      });
    });
  });

  describe('get all messages without user', () => {
    it('returns statusCode 401 and NoUserSpecified', () => {
      request.post(serverUrl + '/getmessagesuser')
      .send({testing: "should be false"})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(401);
        expect(res.text).to.equal('NoUserSpecified');
      });
    });
  });
});

describe('Miscellanious authHandler tests', () => {
  describe('trying to login with incorrect password', () => {
    it('returns 401 and informational body', () => {
      request.post(serverUrl + '/auth/login')
      .send({email: 'a@mail.com', passw: 'fel passw'})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(401);
        expect(res.body.success).to.be.false;
        expect(res.body.message).to.equal('Incorrect password');
      });
    });
  });

  describe('trying to login without password', () => {
    it('returns 401 and informational body', () => {
      request.post(serverUrl + '/auth/login')
      .send({email: 'a@mail.com'})
      .on('response', (res) => {
        expect(res.statusCode).to.equal(401);
        expect(res.body.success).to.be.false;
        expect(res.body.message).to.equal('Authentication failed! Please check the request');
      });
    });
  });
});

//--------Clean-up-dev-db-after-test-------------------------------------------
after((done) => {
  Users.deleteMany({}, function(err) {
    if (err) console.log(err);
  });
  Messages.deleteMany({}, function(err) {
    if (err) console.log(err);
  });
  done();
});
//------------------------------------------------------------------------------
