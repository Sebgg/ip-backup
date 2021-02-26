import React, { Component } from 'react';
import { Redirect, Link } from 'react-router-dom';
import {Helmet} from 'react-helmet';
import {SERVER_URL} from './config';
import './styles/signup.css';

class SignUp extends Component {
  constructor(props) {
    super(props);
    this.state = {
      getBack: [],
      unsuccessfulSignup: false
    }
    this.signUp = this.signUp.bind(this);
  }

  async signUp() {
    let username = document.getElementById('signUpUname').value;
    let email = document.getElementById('signUpEmail').value;
    let password = document.getElementById('signUpPassword').value;

    let inData = {
      uname: username,
      email: email,
      passw: password
    };

    await fetch(SERVER_URL + '/auth/signup', {
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
      body: JSON.stringify(inData) // body data type must match "Content-Type" header
    })
    .then(res => {
      return res.json();
    })
    .then(data => this.setState({getBack: data}))
    .catch(error => console.log(error));
  }

  render() {
    console.log(this.state.getBack.email);
    if (this.state.getBack.success === true) {
      let rM = document.getElementById('rememberMe').checked;
      localStorage.setItem('remember_user', JSON.stringify(rM));
      localStorage.setItem('user_email', JSON.stringify(this.state.getBack.email));
      console.log('Redirecting..');
      this.props.updateLogIn();
      return <Redirect to="/userpage"/>
    } else if (this.state.getBack.success === false){
      this.setState({
        unsuccessfulLogin: true
      });
    }

    return (
      <div className="container">
        <Helmet>
          <title>Sign up</title>
        </Helmet>
        <div className="row justify-content-center">
          <div className="shadow-lg col rounded col-md-4 md-offset-4 justify-content-center" id="loginBox">
              <img src="https://bit.ly/2VJmCe4" alt="..." className="img-thumbnail shadow"/>
              <form id="login">
                <div className="form-group">
                  <input type="text" className="form-control shadow" id="signUpUname" placeholder="Enter username"/>
                </div>
                <div className="form-group">
                  <input type="email" className="form-control shadow" id="signUpEmail" placeholder="Enter email"/>
                </div>
                <div className="form-group">
                  <input type="password" className="form-control shadow" id="signUpPassword" placeholder="Enter password"/>
                </div>
                <button onClick={this.signUp} id="signUpBtn" className="btn btn-primary">Sign up</button>
                <Link to="/login" className="btn btn-secondary float-right">Log in</Link>
                <div className="form-check">
                  <input className="form-check-input" type="checkbox" value="" id="rememberMe"/>
                  <label className="form-check-label" id="toLogin" htmlFor="rememberMe">
                  Remember me, for now
                  </label>
                </div>
              </form>
          </div>
        </div>
      </div>
    )
  }
}

export default SignUp;
