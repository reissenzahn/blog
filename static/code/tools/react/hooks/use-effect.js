import React, { useEffect } from 'react';
import ReactDOM from 'react-dom';

// the useEffect() hook is used to run side-effects

const App = () => {

  // useEffect() takes an effect function and a list of dependencies
  useEffect(() => {
    // the effect function is executed when the component mounts and thereafter only when the value of a dependency changes

  }, []);

  return (
    <div>
    </div>
  );
};

//  Return a cleanup function from the effect that React will run before rerunning
// the effect function and when the component unmounts:
// useEffect(() => {
//  // perform effect
//  return () => {/* clean-up */};
// }, [dep1, dep2]);
//  To avoid race conditions and to follow the convention of returning nothing or a
// cleanup function from the effect function, put async functions inside the effect
// function. You can call them immediately, as necessary:
// useEffect(() => {
//  async function getUsers() {
//  const resp = await fetch(url);
//  const data = await (resp.json());
//  setUsers(data);
//  }
//  getUsers();
// }, []);


ReactDOM.render(<App />, document.getElementById('root'));