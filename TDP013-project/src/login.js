import React, { Component } from 'react';
import { Redirect, Link } from 'react-router-dom';
import {Helmet} from 'react-helmet';
import {SERVER_URL} from './config';
import './styles/login.css';

class Login extends Component {
  constructor(props) {
    super(props);
    this.login = this.login.bind(this);
    this.state = {
      login: [],
      unsuccessfulLogin: false
    };
  }

  login() {
    let email = document.getElementById('signInEmail').value,
        password = document.getElementById('signInPassword').value;


    let inData = {email: email, passw: password};
    console.log(SERVER_URL);
    fetch(SERVER_URL + '/auth/login', {
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
    .then(response => {return response.json();})
    .then(data => {
      this.setState({login: data})
    })
    .catch(error => console.log(error));
  }

  render() {
    let login = this.state.login;
    let eClass = "form-control shadow";
    let pClass = "form-control shadow";
    let eHolder = "Enter email";
    let pHolder = "Enter password";
    if (login !== undefined && login.success === true) {
      let rM = document.getElementById('rememberMe').checked;
      localStorage.setItem('remember_user', JSON.stringify(rM));
      localStorage.setItem('user_email', JSON.stringify(this.state.login.email));
      console.log('Redirecting..');
      this.props.updateLogIn();
      return <Redirect to="/userpage"/>
    } else if (login !== undefined && login.success === false){
      this.setState({
        unsuccessfulLogin: true
      });
    }

    if (this.state.unsuccessfulLogin) {
      if (this.login.message === 'Incorrect password') {
        pClass = pClass.concat(" alert-danger");
        pHolder = "Incorrect password";
      } else {
        eClass = eClass.concat(" alert-danger");
        eHolder = "User not found";
      }
    }

    return (
      <div className="container">
      <Helmet>
        <title>Log in</title>
      </Helmet>
        <div className="row justify-content-center">
          <div className="shadow-lg col rounded col-md-4 md-offset-4 justify-content-center" id="loginBox">
              <img src="https://i.redd.it/td006ond5rn21.jpg" alt="..." className="img-thumbnail shadow"/>
              <form id="login">
                <div className="form-group">
                  <input type="email" className={eClass} id="signInEmail" placeholder={eHolder}/>
                </div>
                <div className="form-group">
                  <input type="password" className={pClass} id="signInPassword" placeholder={pHolder}/>
                </div>
                <button id="loginBtn" onClick={this.login} className="btn btn-primary">Log in</button>
                <Link to="/signup" id="toSignUp" className="btn btn-secondary float-right">Sign up</Link>
                <div className="form-check">
                  <input className="form-check-input" type="checkbox" value="" id="rememberMe"/>
                  <label className="form-check-label" htmlFor="rememberMe">
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

export default Login;
