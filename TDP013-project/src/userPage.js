import React, { Component } from 'react';
import {Link} from 'react-router-dom';
import {Helmet} from 'react-helmet';
import {SERVER_URL} from './config';
import './styles/userpage.css';

// TODO: -fix date formatting.
//       -cleanup
//       -fix remove friend?
class Userpage extends Component {
  constructor(props) {
    super(props);
    this.state = {
      messages: [],
      user: [],
      atFriends: false,
      okToken: true,
      inCorrectInput: false
    };
    this.postMessage = this.postMessage.bind(this);
    this.makeFriend = this.makeFriend.bind(this);
  }

  // use props for saving at friends, from friendsearch
  getMessages() {
    let inData = {}
    if (this.state.atFriends && localStorage.getItem('email')) {
      let email = JSON.parse(localStorage.getItem('email'));
      inData = {fEmail: email};
    }

    fetch(SERVER_URL + '/getmessagesuser', {
      method: 'POST', // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'include', // include, *same-origin, omit
      // xhrFields: {
      //   withCredentials: true
      // },
      headers: {
        'Content-Type': 'application/json'
        // 'Content-Type': 'application/x-www-form-urlencoded',
      },
      redirect: 'follow', // manual, *follow, error
      referrer: 'no-referrer', // no-referrer, *client
      body: JSON.stringify(inData)
    })
    .then(res => {
      if (res.ok) {
        return res.json();
      }else {
        this.setState({
          okToken: false
        })
      }
    })
    .then(data => {
      this.setState({messages: data});
    })
    .catch(error => console.log(error))
  }

  getUserInfo() {
    let inData = {};
    if (this.state.atFriends && localStorage.getItem('email')) {
      let email = JSON.parse(localStorage.getItem('email'));
      inData = {fEmail: email};
    }

    fetch(SERVER_URL + '/user/getinfo', {
      method: 'POST', // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'include', // include, *same-origin, omit
      headers: {
        'Content-Type': 'application/json'
        // 'Content-Type': 'application/x-www-form-urlencoded',
      },
      redirect: 'follow', // manual, *follow, error
      referrer: 'no-referrer', // no-referrer, *client
      body: JSON.stringify(inData)
    })
    .then(res => {
      if (res.ok) {
        return res.json();
      }
    })
    .then(data => this.setState({user: data}))
  }

  makeFriend() {
    let thisUser = JSON.parse(localStorage.getItem('user_email'));
    let friend = JSON.parse(localStorage.getItem('email'));

    let inData = {email: thisUser, femail: friend};

    fetch(SERVER_URL + '/user/addfriend', {
      method: 'POST', // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'include', // include, *same-origin, omit
      headers: {
        'Content-Type': 'application/json'
        // 'Content-Type': 'application/x-www-form-urlencoded',
      },
      redirect: 'follow', // manual, *follow, error
      referrer: 'no-referrer', // no-referrer, *client
      body: JSON.stringify(inData)
    })
    .then(res => {
      if (res.ok) {
        console.log('Updating..');
        return res;
      }
    })
    .catch(err => console.log(err));
  }

  updatePage() {
    let url = window.location.hash;
    let atfriends = (url.indexOf('true') !== -1); // atfriends is saved
    this.setState({atFriends: atfriends}, function() {
      this.getMessages();
      this.getUserInfo();
    })
  }

  UNSAFE_componentWillReceiveProps (nextProps) {
    const { location } = this.props;
    const { location: nextLocation } = nextProps;

    if (location && nextLocation) {
      this.updatePage();
    }
  }

  componentDidMount() {
    this.updatePage();
  }

  renderUserInfo() {
    let button;
    let thisUser = JSON.parse(localStorage.getItem('user_email'));
    const user = this.state.user;
    if(user.friends !== undefined) {
      if (user.email !== thisUser && (this.state.atFriends && !user.friends.includes(thisUser))){
      button = <button className="btn btn-light" onClick={this.makeFriend}>Make friend</button>
      }
    }
    let bClass = 'shadow rounded info'
    try {
      return <div key={user.id} className={bClass}>
              <h2>Username: {user.username}</h2>
              <h2>Email: {user.email}</h2>
              {button}
              <h3>Friends: </h3>
              {user.email === thisUser && (user.friends === undefined || user.friends.length === 0) ? (
                <>
                <h4 id="noFriends">No friends yet!</h4>
                <Link className="btn btn-secondary btn-sm" to="/friendsearch">Go make some!</Link>
                </>
              ) : (
                user.friends.map(friend => {
                  return (
                    <div key={friend} className="shadow rounded friend">
                      <h4 id="friend">{friend}</h4>
                    </div>
                  )
                })
              )}
             </div>
    } catch (err) {
      console.log(err);
    }
  }

  // Tries to render the messages for the userpage user is currently at
  renderMessages() {
    const data = this.state.messages;

    try {
      return data.map(message => {
        return <div key={message._id} className="shadow rounded post">
                <h1 className="shadow rounded postmessage">{message.message}</h1>
                <div className="row">
                  <div className="col col-sm-3 offset-sm-1 postinfo rounded shadow">
                    <p>Created by: {message.author}</p>
                  </div>
                  <div className="col col-sm-3 offset-sm-4 postinfo rounded shadow">
                    <p>Created: {message.date}</p>
                  </div>
                </div>
               </div>
      });
    } catch (err) {
      console.log(err);
    }
  }

  // Lets a user post a message on a userpage
  postMessage() {
    let post = document.getElementById('postInput').value;
    if (post.length === 0 || post.length > 140) {
      this.setState({inCorrectInput: true});
    } else {
      let user = this.state.user;

      let inData = {author: 'cookie', owner: user.email, message: post};

      fetch(SERVER_URL + '/save', {
        method: 'POST', // *GET, POST, PUT, DELETE, etc.
        mode: 'cors', // no-cors, *cors, same-origin
        cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
        credentials: 'include', // include, *same-origin, omit
        // xhrFields: {
        //   withCredentials: true
        // },
        headers: {
          'Content-Type': 'application/json'
          // 'Content-Type': 'application/x-www-form-urlencoded',
        },
        redirect: 'follow', // manual, *follow, error
        referrer: 'no-referrer', // no-referrer, *client
        body: JSON.stringify(inData)
      })
      .then(res => {
        if (res.ok) {
          this.setState({inCorrectInput: false});
          this.getMessages();
        }
      });
    }
  }

  render() {
    let user_email = JSON.parse(localStorage.getItem('user_email'))
    if (this.state.okToken && user_email !== '') {
      let postPlaceholder = "Post something";
      let username = this.state.user.username;
      let iClass;
      if (this.state.inCorrectInput) {
        postPlaceholder = 'Needs to be between 1 and 140 characters';
        iClass = 'form-control shadow alert-danger';
      } else {
        iClass = 'form-control shadow';
      }
      return (
        <div className="container-fluid">
        <Helmet>
          <title>{username}</title>
        </Helmet>
          <div className="row" id="content">
            <div className="col col-sm-4">
              <div className="row">
                <div className="col col-sm-12 shadow rounded" id="personalInfo">
                  {this.renderUserInfo()}
                </div>
              </div>
            </div>
            <div className="col col-sm-7 offset-sm-1">
              <div className="row">
                <div className="col col-sm-12 shadow rounded" id="fWrite">
                  <div className="input-group mb-3">
                    <input type="text" className={iClass} placeholder={postPlaceholder} aria-label={postPlaceholder} aria-describedby="post-button" id="postInput" disabled={!this.state.atFriends}/>
                    <div className="input-group-append shadow">
                      <button className="btn btn-secondary" type="button" id="post-button" onClick={this.postMessage} disabled={!this.state.atFriends}>Post</button>
                    </div>
                  </div>
                </div>
                <div className="col col-sm-12 shadow rounded" id="posts">
                  {this.state.messages === [] ?
                    <p>Loading data</p>
                    :
                    this.renderMessages()
                  }
                </div>
              </div>
            </div>
          </div>
        </div>
      )
    } else {
      return (
        <div className="container">
          <div className="row" id="content">
            <div className="col col-sm-8 sm-offset-4 shadow rounded justify- content-center" id="noToken">
              <img src="https://i1.sndcdn.com/artworks-000374013141-1yvj3i-t500x500.jpg" alt="..." className="rounded"/>
              <h1>
                Oops! Seems you are not logged in
              </h1>
            </div>
          </div>
        </div>
      )
    }
  }
}

export default Userpage;
