

window.onload = function get_body() {

  const quotes = [
    '“Take care of the sense and the sounds will take care of themselves.”<br>— Lewis Carroll',
    '“I will speak ill of no man and speak all the good I know of everybody.”<br>— Benjamin Franklin',
    '“The world is indeed comic, but the joke is on mankind.”<br>— H.P. Lovecraft',
    '“If I have seen further it is by standing on the shoulders of Giants.”<br>— Isaac Newton',
    '“I am hearty in my appreciation and lavish in my praise.”<br>— Charles Schwab',
    '“Never criticize, condemn or complain.”<br>— Dale Carnegie',
    '“Freedom (n.): To ask nothing. To expect nothing. To depend on nothing.”<br>— Ayn Rand',
    '“He that would keep a secret must keep it secret, that he hath a secret to keep.”<br>— Humphrey Appelby',
    '“We buy balloons, we let them go.”<br>— Bret Easton Ellis',
    '“Silence is more eloquent than words.”<br>— Thomas Carlyle',
    '“No matter what you look at, if you look at it closely enough, you are involved in the entire universe.”<br>— Faraday Michael',
  ];
  
  document.getElementById('quote').innerHTML = pick(quotes);
  
  // '“”<br>— ',


  // const colors = [
  //   '#f5fffa',
  //   '#fff5ee',
  //   '#f0f8ff',
  //   '#fff8e7',
  //   '#ece9d1',
  // ];

  // document.body.style.backgroundColor = pick(colors);
}


const pick = (array) => {
  return array[Math.floor(Math.random() * array.length)];
}
