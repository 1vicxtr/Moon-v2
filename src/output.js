function Header(root) { 
  let e1 = document.createElement('h1');
  root.appendChild(e1);
  e1.textContent = "🌙 Moon Dashboard";
}

function UserList(root) { 
  let e1 = document.createElement('ul');
  e1.setAttribute("id", "user-list");
  root.appendChild(e1);
  // Bloque JS crudo
  
        const users = ["Alice", "Bob", "Charlie"];
        const ul = document.getElementById("user-list");

        users.forEach(u => {
            const li = document.createElement("li");
            li.textContent = u;
            li.addEventListener("click", () => alert("Usuario: " + u));
            ul.appendChild(li);
        });
    ;
}

function Footer(root) { 
  let e1 = document.createElement('footer');
  root.appendChild(e1);
  e1.textContent = "Moon Dashboard © 2025";
}

function Counter(root) { 
  let e1 = document.createElement('div');
  e1.setAttribute("id", "counter-container");
  root.appendChild(e1);
  let e2 = document.createElement('p');
  e2.setAttribute("id", "counter-value");
  e1.appendChild(e2);
  e2.textContent = "0";
  // Bloque JS crudo
  
        let count = 0;
        const value = document.getElementById("counter-value");

        const incBtn = document.createElement("button");
        incBtn.textContent = "Incrementar";
        incBtn.addEventListener("click", () => {
            count++;
            value.textContent = count;
        });

        const decBtn = document.createElement("button");
        decBtn.textContent = "Decrementar";
        decBtn.addEventListener("click", () => {
            count--;
            value.textContent = count;
        });

        document.getElementById("counter-container").appendChild(incBtn);
        document.getElementById("counter-container").appendChild(decBtn);
    ;
}

