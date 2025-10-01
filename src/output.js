function Home(root) {
  let e1 = document.createElement('h1');
  e1.setAttribute("class", "clase");
  root.appendChild(e1);
  e1.textContent = "Hello is text h1!";
  let e2 = document.createElement('h2');
  e2.setAttribute("class", "clase2");
  root.appendChild(e2);
  e2.textContent = "Helloo is text h2!";
  let e3 = document.createElement('div');
  root.appendChild(e3);
  let e4 = document.createElement('h3');
  e4.setAttribute("class", "clase3");
  e3.appendChild(e4);
  e4.textContent = "Hellooo is text h3!";
}

function App(root) {
  let e1 = document.createElement('h1');
  e1.setAttribute("class", "App");
  root.appendChild(e1);
  e1.textContent = "Hello is App!";
}

function Footer(root) {
  let e1 = document.createElement('p');
  root.appendChild(e1);
  e1.textContent = "Lorem";
}

