import React, {Component} from 'react';
import {Helmet} from 'react-helmet';

// not yet implemented

class Page400 extends Component {
  render() {
    return (
      <div className="container">
        <Helmet>
          <title>500 error</title>
        </Helmet>
        <div className="row self-adjust-center">
          <div className="col-8">
            <p>Oops, seems you did something bad!</p>
          </div>
        </div>
      </div>
    )
  }
}
export default Page400;
