import React, { Component } from 'react';
import {Link} from 'react-router-dom';
import {Helmet} from 'react-helmet';
import {SERVER_URL} from './config';
import './styles/friendsearch.css';

class FriendSearch extends Component {
  constructor(props) {
    super(props);
    this.search = this.search.bind(this);
    this.state = {
      searchR: [],
      user: [],
    };
  }

  search() {
    let queryData = document.getElementById('emailSearch').value;

    let inData = {query: queryData};
    fetch(SERVER_URL + '/user/finduser', {
      method: 'POST', // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'same-origin', // include, *same-origin, omit
      headers: {
        'Content-Type': 'application/json'
        // 'Content-Type': 'application/x-www-form-urlencoded',
      },
      redirect: 'follow', // manual, *follow, error
      referrer: 'no-referrer', // no-referrer, *client
      body: JSON.stringify(inData) // body data type must match "Content-Type" header
    })
    .then(res => {
      if (res.ok) {
        return res.json();
      }
      // else if(res.status >= 400 && res.status < 500) {
      //   return <Redirect to='/400'/>;
      // } else {
      //   return <Redirect to='/500'/>;
      // }
    })
    .then(data => this.setState({searchR: data}))
    .catch(error => console.log(error));
  }

  getUserInfo() {
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
      referrer: 'no-referrer' // no-referrer, *client
    })
    .then(res => {
      if (res.ok) {
        return res.json(); // test errorhandling
      } else {
        throw new Error(res.status);
      }
    })
    .then(data => this.setState({user: data}))
    .catch(err => console.log(err));
  }

  componentDidMount() {
    this.getUserInfo()
  }

  checkIfFriend(friend, friends) {
    if (friends === undefined) {
      return;
    } else {
      return <Link to={{pathname: '/friendpage/'+friend.email+'/true',}} className="btn btn-secondary" onClick={() => localStorage.setItem('email', JSON.stringify(friend.email))}>Go to {friend.username}'s page</Link>;
    }
  }

  renderSearchResult() {
    let data = [];
    let friends = null;
    // eslint-disable-next-line
    if (typeof this.state.searchR === Array) {
      this.state.searchR.map(res => {data.push(res)});
    } else {
      data = this.state.searchR;
    }
    if (this.state.user) {
      friends = this.state.user.friends;
    }

    if (data === undefined || data.length === 0 || friends === null) {
      return <div className="shadow rounded" id="noResults">
              <h2>No results found</h2>
             </div>
    } else if (data._id){ // check for refactoring
      return <div key={data._id} className="shadow rounded resultOne">
              <h2>Username: {data.username}</h2>
              <h3>Email: {data.email}</h3>
              {this.checkIfFriend(data, friends)}
             </div>
    } else {
      return data.map(user => {
        return <div key={user._id} className="shadow rounded resultMany">
                <h2>Username: {user.username}</h2>
                <h3>Email: {user.email}</h3>
                {this.checkIfFriend(user, friends)}
               </div>
      });
    }
  }

  render() {
    return (
      <div className="container">
        <Helmet>
          <title>Search</title>
        </Helmet>
        <div className="row">
          <div className="col col-sm-10 offset-sm-2 shadow rounded" id="input-div">
            <form>
              <div className="form-inline">
                <input type="text" className="form-control" id="emailSearch" aria-describedby="emailHelp" placeholder="Enter email or username"/>
                <button type="submit" className="btn btn-primary" onClick={this.search}>Search</button>
              </div>
            </form>
          </div>
          <div className="col col-sm-10 offset-sm-2 shadow rounded" id="result-div">
            {this.renderSearchResult()}
          </div>
        </div>
      </div>
    )
  }
}

export default FriendSearch;
