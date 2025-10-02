# Moon 

This repository was created for Moon, a React-inspired language. Moon doesn't require any libraries or servers; everything you need is in this repository.

## Description

Just as the moon has phases, front-end web development also evolves in different stages. That's why the name Moon fits perfectly. You don't need servers or external programs: everything you need is right here, in this folder.


## Installation

1. Clone my repository: 
 git clone https://github.com/1vicxtr/Moon.git


## How to use the compiler

    The directory will be like this:
    Moon -> 
    component ->
        This is where the .mn files will be
    src ->
        This is where the C compiler will be
    index.html
    style.css
    README.md

- Windows: Type the moon syntax and to run it just run the parser.exe executable.
- Linux: just use this command ./parser


# Syntax 


Moon's syntax is very friendly.
~~~~
// Home.mn
component Home{
    <html>
        //Code HTML
        <h1>Hello Word!</h1>
    </html>
    js(
        //Code JavaScript
        alert("Hello word!");
    )
}
~~~~



***
## Version
Moon is in its first quarter phase (v1.1), so you can now write HTML tags and text inside them. In addition to declaring variables (int and string), I'm of course working on improving the syntax.