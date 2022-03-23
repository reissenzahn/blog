import React, { useReducer } from 'react';
import ReactDOM from 'react-dom';

// the useReducer hook provides a way to manage multiple pieces of interrelated state

const ADD = 'ADD';
const SUBTRACT = 'SUBTRACT';
const RESET = 'RESET';

// a reducer is a function that takes the current state and an action and uses the action to return a new state
const reducer = (state, action) => {
  // the reducer typically returns a new state depending on the type of the action
  switch (action.type) {
    case ADD:
      return state + action.payload;
    case SUBTRACT:
      return state - action.payload;
    case RESET:
      return 0;
    default:
      return state;
  }
};

const App = () => {
  // a call to useReducer() takes a reducer and an initial state as arguments and returns an array containing the state and a dispatch function
  const [state, dispatch] = useReducer(reducer, 0);

  const add = (amount) => {
    // the dispatch() function will pass the current state and the action provided in its argument to the reducer and replace the state with the new state returned by the reducer
    dispatch({ type: ADD, payload: amount });
  };

  const reset = () => {
    dispatch({ type: RESET });
  };

  return (
    <div>
      <span>{state}</span>
      <button onClick={() => add(1)}>add</button>
      <button onClick={reset}>reset</button>
    </div>
  );
};

ReactDOM.render(<App />, document.getElementById('root'));