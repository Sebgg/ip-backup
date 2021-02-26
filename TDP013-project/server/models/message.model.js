var mongoose = require("mongoose"),
    Schema = mongoose.Schema;
const mp = require("mongoose-paginate");

var messageSchema = new Schema({
  author: {type: String, required: 'AuthorInvalid'}, //saves email of author
  owner: {type:String, required: 'OwnerInvalid'}, //saves email address of owner
  message: {type: String, required: 'MessageInvalid'},
  // readBy: [String], // Saves userid for those who jh
  date: {type: Date, default: Date.now}
});

messageSchema.plugin(mp);

module.exports = mongoose.model('message', messageSchema);
