import React, { memo, useState, useRef, useCallback } from 'react';
import ReactDOM from 'react-dom';

// the useCallback() hook is used to maintain the identity of functions across renders

const Display = memo(({ increment }) => {
  const renders = useRef(0);

  console.log('renders: ', renders.current++);

  return (
    <button onClick={increment}>+</button>
  );
});

const App = () => {
  const [count, setCount] = useState(0);

  // the useCallback() hook 

  return (
    <div>
      <span>{count}</span>
      <Display increment={() => setCount((count) => count + 1)}/>
    </div>
  );
};

//  Maintain the identity of functions across renders by wrapping them in calls to
// the useCallback hook. React will redefine the function only when the dependencies change:
// const stableFunction = useCallback(functionToCache, dependencyList);

ReactDOM.render(<App />, document.getElementById('root'));