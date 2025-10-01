# Moon 

This repository was created for Moon, a React-inspired language. Moon doesn't require any libraries or servers; everything you need is in this repository.

## Description

Just as the moon has phases, front-end web development also evolves in different stages. That's why the name Moon fits perfectly. You don't need servers or external programs: everything you need is right here, in this folder.


## Installation

1. Clone my repository: 
 git clone https://github.com/1vicxtr/Moon.git
2. It will give you my folder
In index.html, the bundle.js file, which is the Moon interpreter, is already connected.
3. In the /compiler folder are the lexers and parsers (compilers)
4. The .mn (moon) files will go in the components/ folder.

## How to use the compiler

- If you are using Windows, just run moon-compiler (if it doesn't work, contact me).
- If you are using Linux, just use this command ./watcher


# Syntax 


Moon's syntax is very friendly.
~~~~
// Home.mn
component Home{
    <h1 class="clase">Hello is text h1!</h1>
    <h2 class="clase2">Helloo is text h2!</h2>
    <div>
        <h3 class="clase3">Hellooo is text h3!</h3>
    </div>
}

~~~~

***
## Version
Moon is in its first quarter phase (v1.1), so you can now write HTML tags and text inside them. In addition to declaring variables (int and string), I'm of course working on improving the syntax.