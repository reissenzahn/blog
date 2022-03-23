import React, { useState, useMemo } from 'react';
import ReactDOM from 'react-dom';

// the useMemo() hook is used to avoid re-running expensive computations

// a call to this slow function will be made each time the component re-renders so we use useMemo() to memoize its result
const slowDouble = (num) => {
  console.log('working...');

  for (let i = 0; i <= 1000000000; i++) {}
  
  return num * 2;
};

const App = () => {
  const [count, setCount] = useState(0);
  const [, rerender] = React.useState();

  // a call to useMemo() takes the function to memoize and a list of dependencies that should cause the function to be re-executed should they change
  const doubleCount = useMemo(() => slowDouble(count), [count]);

  return (
    <div>
      <span>{doubleCount}</span>
      <button onClick={() => setCount((count) => count + 1)}>increment</button>
      <button onClick={rerender}>refresh</button>
    </div>
  )
};

ReactDOM.render(<App />, document.getElementById('root'));