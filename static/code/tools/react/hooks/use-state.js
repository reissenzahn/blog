import React, { useState } from 'react';
import ReactDOM from 'react-dom';

// the useState hook is used to  declare a state variable

const App = () => {
  // a call to useState() takes an initial value as an argument and returns an array containing the state value and an updater function
  const [count, setCount] = useState(0);

  // a function can be passed for the initial value in which case React will run the function to get the initial state only when it first calls the component
  // const [count, setCount] = useState(() => {
  //   console.log('working...');
  //   return 0;
  // };

  const reset = () => {
    // the updater function can be called to replace the current value with a new value which will trigger a re-render
    setCount(0);
  };

  const add = (amount) => {
    // if the new value depends on the previous value then a function should be passed which will receive the previous value as its argument in order to prevent operating on stale state
    setCount((count) => count + amount);
  };

  return (
    <div>
      <span>{count}</span>
      <button onClick={() => add(1)}>add</button>
      <button onClick={reset}>reset</button>
    </div>
  );
};

ReactDOM.render(<App />, document.getElementById('root'));