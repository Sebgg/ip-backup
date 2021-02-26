import React , { Component } from 'react';
import {Helmet} from 'react-helmet';

class Home extends Component {
  render() {
    return (
      <div className="container">
        <Helmet>
          <title>Home</title>
        </Helmet>
        <div className="row justify-content-center">
          <div className="col-8 md-offset-6 justify-content-center">
            <img src="https://i.kym-cdn.com/photos/images/original/000/814/003/692.jpg" className="img-fluid" alt="Responsive"/>
          </div>
        </div>
      </div>
    )
  }
}

export default Home;
