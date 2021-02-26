var mongoose = require("mongoose");
const bcrypt = require("bcryptjs"),
      mp = require("mongoose-paginate"),
      schema = mongoose.Schema,
      mongooseHidden = require('mongoose-hidden')({ defaultHidden: { password: true } });

var userSchema = new schema({
  username: {type: String, required: 'UserNameInvalid'},
  email: {type: String, unique: true, lowercase: true, required: 'EmailInvalid'},
  password: {type: String, required: 'PasswordInvalid'},
  friends: [String] // saves email for friends
});

userSchema.plugin(mp);

userSchema.pre('save', function(next) {
  var user = this;
  if (!user.isModified('password')) {
    return next();
  }
  bcrypt.genSalt(10, function(err, salt) {
    bcrypt.hash(user.password, salt, function(err, hash) {
      user.password = hash;
      next();
    });
  });
});

userSchema.methods.comparePassword = function(password) {
  let match = bcrypt.compare(password, this.password);
  return match;
};

userSchema.plugin(mongooseHidden); // lets me hide password outside of server

module.exports = mongoose.model('User', userSchema);
