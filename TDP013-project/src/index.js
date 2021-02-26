import React , { Component } from 'react';
import ReactDom from 'react-dom';
import Navbar from './navbar';
import './styles/index.css';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      remember: false
    }
  }

  handleRemember() {
    let rememberMe = JSON.parse(localStorage.getItem('remember_user'));
    this.setState({remember: rememberMe});
  }

  async componentDidMount() {
    localStorage.setItem('email', JSON.stringify(''));
    await this.handleRemember();
    if (this.state.remember === false) {
      localStorage.setItem('user_email', JSON.stringify(''));
    }
  }

  render() {
    return (
      <div className="App">
        <Navbar/>
      </div>
    )
  }
}

ReactDom.render(<App />, document.getElementById('root'));
