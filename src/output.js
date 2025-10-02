function Home(root) { 
  let e1 = document.createElement('h1');
  root.appendChild(e1);
  e1.textContent = "Home!";
}

function App(root) { 
  let e1 = document.createElement('div');
  root.appendChild(e1);
  let e2 = document.createElement('h1');
  e1.appendChild(e2);
  e2.textContent = "Hola!";
  let e3 = document.createElement('div');
  e1.appendChild(e3);
  let e4 = document.createElement('h2');
  e3.appendChild(e4);
  e4.textContent = "Holaa!";
}

