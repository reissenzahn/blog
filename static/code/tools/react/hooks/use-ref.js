import React, { useEffect, useState, useRef } from 'react';
import ReactDOM from 'react-dom';

// the useRef() hook is used to manage a state value without changes to the value triggering a re-render

const App = () => {
  const [text, setText] = useState('');

  // a call to useRef() takes an optional initial value and returns an object with a current property 
  const renderCount = useRef(1);
  
  // elements have a ref attribute which can be used to automatically assign DOM element references to the current property of a ref
  const inputRef = useRef();

  // an useful application of useRef() is to store the previous value of some state
  const prevText = useRef('');

  useEffect(() => {
    // the current value is persisted across renders and can be directly assigned to
    renderCount.current = renderCount.current + 1;

    console.log(renderCount);
  });

  useEffect(() => {
    prevText.current = text;
  }, [text]);

  const focus = () => {
    inputRef.current.focus();
  }

  return (
    <div>
      <input ref={inputRef} value={text} onChange={e => setText(e.target.value)} />
      <button onClick={focus}>focus</button>
      <div>{prevText.current}</div>
      <div>{text}</div>
    </div>
  )
};

ReactDOM.render(<App />, document.getElementById('root'));