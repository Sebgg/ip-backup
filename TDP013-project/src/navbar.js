import React , { Component } from 'react';
import {
  Route,
  NavLink,
  HashRouter,
  Switch
} from 'react-router-dom';
import Home from './home';
import Login from './login';
import Userpage from './userPage';
import FriendSearch from './friendSearch';
import SignUp from './signup';
import Page500 from './page500';
import Page400 from './page400';
import {SERVER_URL} from './config';
import './styles/navbar.css';

class Navbar extends Component {
  constructor(props) {
    super(props);
    this.state = {
      logIn: false
    }
    this.logout = this.logout.bind(this);
    this.updateLogIn = this.updateLogIn.bind(this);
  }

  componentDidMount() {
    this.updateLogIn();
  }

  updateLogIn() {
    let email = JSON.parse(localStorage.getItem('user_email'));
    let isLoggedIn;
    if (email === '') {
      isLoggedIn = false;
    } else {
      isLoggedIn = true;
    }
    this.setState({logIn: isLoggedIn});
  }

  logout() {
    console.log('logout');
    this.setState({logIn: false});
    localStorage.setItem('remember_user', JSON.stringify(false));
    localStorage.setItem('user_email', JSON.stringify(''));
    localStorage.setItem('email', JSON.stringify(''));
    fetch(SERVER_URL + '/removetoken', {
      method: 'POST', // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'include', // include, *same-origin, omit
      headers: {
        'Content-Type': 'application/json'
        // 'Content-Type': 'application/x-www-form-urlencoded',
      },
      redirect: 'follow', // manual, *follow, error
      referrer: 'no-referrer'
    }).then(res => {
      return;
    });
  }

  render() {
    let loggedIn = this.state.logIn;
    let logButton;

    if (loggedIn === false) {
      logButton = <NavLink className="nav-link" id="login" to="/login">Login</NavLink>
    } else {
      logButton = <NavLink className="nav-link" id="logout" to="/login" onClick={this.logout}>Logout</NavLink>
    }

    return (
      <HashRouter>
        <nav className="navbar sticky-top navbar-expand-lg navbar-light shadow">
          <a className="navbar-brand" href="#top">Twiddel</a>
          <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarToggler" aria-controls="navbarTogglerDemo01" aria-expanded="false" aria-label="Toggle navigation">
            <span className="navbar-toggler-icon"></span>
          </button>
          <div className="collapse navbar-collapse" id="navbarToggler">
            <ul className="navbar-nav mr-auto mt-2 mt-lg-0">
              <li className="nav-item">
                <NavLink className="nav-link" id="home" to="/">Home</NavLink>
              </li>
              <li className="nav-item">
                {logButton}
              </li>
              <li className="nav-item">
                <NavLink className="nav-link" id="upage" to="/userpage">My Page</NavLink>
              </li>
              <li className="nav-item">
                <NavLink className="nav-link" id="search" to="/friendsearch">Search</NavLink>
              </li>
            </ul>
          </div>
        </nav>
        <Switch>
          <Route exact path="/" component={Home}/>
          <Route path="/userpage" component={Userpage}/>
          <Route path="/friendpage/:email/:atfriends" component={Userpage}/>
          <Route path="/friendsearch" component={FriendSearch}/>
          <Route path="/login" render={(props) => <Login {...props} updateLogIn={this.updateLogIn}/>}/>
          <Route path="/signup" render={(props) => <SignUp {...props} updateLogIn={this.updateLogIn}/>}/>
          <Route path="/500" component={Page500}/>
          <Route path="/400" component={Page400}/>
        </Switch>
      </HashRouter>
    );
  }
}

export default Navbar;
