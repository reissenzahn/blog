import React, { useState, createContext, useContext } from 'react';
import ReactDOM from 'react-dom';

// createContext() take an optional default value and creates a context object to manage a particular value that components will access
const ThemeContext = createContext(true);

const Component = () => {
  // the useContext() hook is used to access a context value and will cause a re-render whenever the value changes
  const theme = useContext(ThemeContext);

  return (
    <div>{theme ? 'dark' : 'light'}</div>
  )
};

const App = () => {
  const [theme, setTheme] = useState(false);

  const toggleTheme = () => {
    setTheme((theme) => !theme);
  };

  // we wrap the component tree that needs access to the shared state value with the provider component of the context object and set the value as a prop
  return (
    <ThemeContext.Provider value={theme}>
      <button onClick={toggleTheme}>Toggle Theme</button>
      <Component />
    </ThemeContext.Provider>
  )
};

ReactDOM.render(<App />, document.getElementById('root'));