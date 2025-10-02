# Architecture
Moon's architecture is a beautiful thing because it's so simple and fast.
Below, I'll show you how each file and folder works.
## Compiler/
- #### lexer.c
    The lexer reads all files found in the component folder and the files must always have the .mn extension.
- #### parser.c
    The parser works in conjunction with the lexer, calling the lexer function and validating each character. After that, it translates it into JS code and finally sends it to the output.js file.
## components/
- #### Header.mn
    ```Moon
    /* They will always have the word "component" in addition to the file name itself, in this case it is Header */

        component Header {
        <html>
            /* This <html</html> tag will be necessary to define that you are writing HTML tags. Without it, the compiler will give you an error or won't run. You can also write all the tags you want inside it. */

            <h1>🌙 Moon Dashboard</h1> // html tag
        </html>
    }
    ```
Another example:   
- #### Counter.mn
    ```Moon

    component Counter {
    <html>
        <div id="counter-container">
            <p id="counter-value">0</p>
        </div>
    </html>
    /* To write js code you must put js() and inside the parentheses you must put js code */
    js(
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
        )
    }
    ```
## src/
- ### output.js
    Every time you have a component, the compiler will always leave something like this in output.js:
    ```javascript
        function Footer(root) { 
            let e1 = document.createElement('footer');
            root.appendChild(e1);
            e1.textContent = "Moon Dashboard © 2025";
        }
    ```
    You don't need to change anything since the one that calls the function of said component is index.js

- ### index.js
    Whenever you run the compiler (parser) the file will look like this:
    ```javascript
        function init(){
            const root = document.body;
            Header(root);
            UserList(root);
            Footer(root);
            Counter(root);
        }
        init();
    ``` 
    What this function does is call the functions of the components, if you want to change the order just move the function up or down. You should not remove the root variable since it is what renders the functions.

## bin/
Only the executables will be found here. If you're using Linux, you'll need to run something like this from the bin/ folder:
```console
    ./parser
```