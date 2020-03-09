
---
author: Volker Aßmann (volker.assmann@gmail.com)
date: 12.03.2020
title: Rust - Short Intro, Ownership ![](rust-logo-blk.svg)

---

Hello World
-----------
```rust
fn main()
{
    println!("Hello, World")
}
```

Hello World, with Lifetimes!
--------
```rust
fn hello_world() -> &'static str
{
    "Hello, World!"
}

fn main() {
    println!("{}", hello_world());
}
```

Hello World, with Parameters!
--------

```rust
fn hello(name: &str) -> String
{
    format!("Hello, {}!", name)
}

fn main() {
    println!("{}", hello("C++ User Group"));
}
```

Hello World, with real Strings!
--------
```rust
fn hello(name: &str) -> String
{
    format!("Hello, {}!", name)
}

fn main() {
    let name = String::from("Rust");
    println!("{}", hello(&name));
}
```

What is Rust?
----

* System language like C++, started at Mozilla
* Main focus
    * Safety (memory, threading ...)
    * Speed (same league as C/C++)
    * Good compiler errors
* Very new language (1.0: 2015)
    * Rapid development, e.g. new futures/async
    * "[Editions](https://doc.rust-lang.org/edition-guide/editions/index.html)" to stabilize language
    * Currently Edition 2015 and 2018


Rust Use Cases
----

* Firefox - Servo, CSS, Media/Video Engines
* Web Dev / APIs: Actix / Rocket, Tokio project
* System / GUI Programming (Gtk, SDL)
* Embdeded: Cortex M*, ESP32 ... [rust-embedded book](https://rust-embedded.github.io/book)
* Compile to Webassembly
* (Writing Linux Kernel Modules)
* ... [awesome-rust](https://github.com/rust-unofficial/awesome-rust)


Language Features
----
* Syntax: similar to C/C++, ML, Haskell
* Ownership / lifetime checking for memory safety
* Strong static type system
* Generics
* "Hygienic" Macros (restricted syntax manipulationx, contexts)
* New approach to OO: no inheritance, traits!

Tooling
---
* Compiler - rustc, based on LLVM
* `rustup` for release management
    * stable/beta/nightly releases
    * cross compilers
* Build tool / packaging: cargo + [crates.io](https://crates.io)
* rustfmt, rustdoc
* IDEs: Vim, VS Code ("Racer" plugin), CLion ...
```bash     
cargo new --bin MyProject
cargo build
cargo run
```

Great Documentation
----
* On doc.rust-lang.org
    * [Learning Rust Overview](https://www.rust-lang.org/learn)
    * [Rust Programming Language](https://doc.rust-lang.org/book/title-page.html)
    * [Rust By Example](https://doc.rust-lang.org/stable/rust-by-example)
    * [Rust Reference](https://doc.rust-lang.org/reference/index.html)
    * [Rustonomicon](https://doc.rust-lang.org/nomicon/) (The Dark Arts of Unsafe Rust)
* Others
    * [Rust Cookbook](https://rust-lang-nursery.github.io/rust-cookbook)
    * [Async Programming in Rust](https://rust-lang.github.io/async-book)
    * [Rustlings](https://github.com/rust-lang/rustlings)
    

Data Types - Primitive Types
----
* `bool`, `char` (4 byte char)
* Scalars
    * Integer: `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `isize`, `usize` (default `i32`)
    * Floats:  `f32`, `f64` (default `f64`)
    ```rust
    let x = 2.0; // f64
    let y: f32 = 3.0; // f32
    ```

Variables
----
```rust
let x = 42; // immutable integer
let mut y = 1; // mutable integer
```

Data Type - Compound Types
----
```rust
// Array
let array: [i32; 5] = [0,1,2,3,4];
// Slice
let slice = &array[0..3]; // 0,1,2
// str - a string slice
let s = "hello";
// Tuple
let tup = ("hello", 42, "world", [1,2,3]);
// Vec (like std::vector)
let v = vec![1,2,3];
// String - UTF-8 encoded growable string, like Vec<char>
let s = String::from("💖");
let v = vec![0xF0, 0x9F, 0x98, 0x88];
let t = String::from_utf8(v).unwrap();
```

Data Types - Structs
----
```rust
struct Nil; // empty
struct Pair(i32, i64); // tuple struct (named tuple)

#[derive(Debug)]
struct Person<'a> { // Struct with two fields and lifetime
    name: &'a str,
    age: u8
}
impl Person<'_> { // anonymous lifetime
    fn greet(&self) -> String {
        format!("Hello, {}", self.name)
    }
}
println!("{}", Person{name: "Isabelle", age: 1}.greet());
```

Control Flow
----
```rust
if x % 2 == 0 { println!("x is even!"); } 
else { println!("x is odd!"); }

// infinite loop without break
loop { println!("I'm alive!"); break; } 

let mut i = 0;
while i < 10 { println!("hello"); i = i + 1; }

let mut x = vec![1,2,3];
while let Some(y) = x.pop() {
    println!("y = {}", y);
}
```

Control Flow - iterator loops
----
```rust
let v = &["C++", "Rust", "Go", "Python"];
for text in v {
    println!("I like {}.", text);
}
for (i, text) in v.iter().enumerate() {
    println!("{}: {}", i, text); 
}

let mut sum = 0;
for n in 1..11 {
    sum += n;
}
```

Functions
----

```rust
fn negate(i: i32) -> i32 { -i }

use std::fmt;
pub fn func_name<T: fmt::Display>(arg1: T) -> String {}
{
    println!("{}", arg1); // guaranteed to work by Type bound
    format!("{}", arg1)   
}

// negate only accessible in same module
```

Lambdas
----
```rust
// lambda:
let a = "Hello"; let b = ", world";

// types inferred, b captured:
let x = |in| String::from(in) + b;

// with type annotations:
let y = |in: &str| -> String { ... }

println!("{}", x(a));
```

Data Types - Constructors
----
* "Constructors" just a convention!
```rust
impl<T> Vec<T> {
    pub fn new() -> Vec<T> { /* ... */ }
}
```
```rust
use vec::Vec;
let mut v = Vec::new();
// actually, for Vec there is a macro:
let mut w = vec![1,2,3];
```

Enums
----
Much more powerful than in C++

```rust
enum Message {
    Quit,
    ChangeColor(i32, i32, i32),
    Move {x: i32, y: i32},
    Write(String)
}
let x: Message = Message::Move{ x: 1, y: 2};
```

Matching
----
`match` is like a powerful switch / `if` cascade

```rust
let x = 2;
let as_string = match x {
    1 => "one",
    2 => "two",
    _ => "sorry, can only count to two..."
};
```
Match needs to be *exhaustive* (thus the `_`)

Matching and Enums
----
Bind values from variant for use in match

```rust
fn receive(msg: Message) {
    match msg {
        Message::Quit => panic!("we want to leave FAST!"),
        Message::ChangeColor(r,g,b) => change_color(r,g,b),
        Message::Move {x: x, y: y} => move_cursor(x, y),
        Message::Write(s) => println!("{}", s),
    }
}
```

Error Matching
----
* Rust uses std::result for Error Handling (no exceptions)

```rust
#[must_use]
enum Result<T, E> {
    Ok(T),
    Err(E)
}
let x = "4711".parse::<u8>(); // from trait std::str::FromStr
// let y = x.unwrap(); -- will panic if error, often found in examples
match x {
    Ok(i) => println!("The number is {}", i),
    Err(e) => println!("{:?}", e), // remember #[derive(Debug)] ?
}
// Output: ParseIntError { kind: Overflow }
```

Error Handling Shortcut
----
```rust
use std::num::ParseIntError;
fn may_fail(in: i32) -> Result<i32, ParseIntError> {
    return in.parse<i32>()?;
    // or
    return try!(in.parse<i32>())
    // same as
    let num = match in.parse() {
        Ok(number) => number,
        Err(e) => return Err(e)
    };
    return num;
}
```

Traits
----
```rust
trait Print {
    fn print(&self) -> String;
}

impl Print for i32 {
    fn print(&self) -> String {
        format!("Int({})", self)
    }
}
impl Print for f64 {
    fn print(&self) -> String { ... }
}
```
Impl only allowed in either trait module or type module!

Trait Objects
----
```rust
let x = 42;
let y = 3.14;
let v: Vec<&dyn Print> = vec![&x, &y];

for it in v { println!("Item: {}". it.print()); }

fn do_static<T: Print>(x: T) { println!("{}", x.print()); }
fn do_dynRef(x: &dyn Print) { println!("{}", x.print()); }
fn do_dynPtr(x: Box<dyn Print>) { println!("{}", x.print()); }

do_static(x);
do_dynRef(&y);
do_dynPtr(Box::new(666));
```

Wraper (Pointer) Types
----
* `Box<T>` - single ownership pointer (unique_ptr)
* `Rc<T>` - reference counted pointer
* `Arc<T>` - reference counted thread safe pointer (shared_ptr)
* `Cell<T>` / `RefCell<T>` - provide "interior mutability" for the wrapped type

```rust
let x = RefCell::new(vec![1,2,3]);
{
    println!("{:?}", *x.borrow())
}
{
    let my_ref = x.borrow_mut();
    my_ref.push(1);
}
```


Modules
----
* Powerful hierarchical module system
    * Should map to filesystem
* Base for visibility
```rust
mod my_module {
    pub fn foo() {}
    fn bar() {}
}
use my_module;
use my_module::foo as fump;
use deeply::nested::{a_func, ATrait};
```

Safety
----
* Two languages: SAFE Rust and UNSAFE Rust
    * Safe Rust - Compiler ENFORCES lifetimes and sharing rules, sane defaults:
        * Move by default, copy always explicit
        * Variables / references immutable by default
        * No UB, automated bounds / integer checks, PANIC on UB
    * Unsafe Rust - disable checks in unsafe {} scope
        * Semantic: "the developer has verified that this works!"
        * E.g. unsafe traits Send / Sync

Memory Safety: Microsoft
----
![](Microsoft_Memory_Safety_CVEs.png)

Ownership
----
* Three ownership rules:
    * Each vaule in Rust has a variable that's called its _owner_
    * There can only be one owner at a time
    * When the owner goes out of scope, the value will be dropped

* All rules enforced at compile time - no runtime overhead!
    * But learning overhead: "fighting the borrow checker"

References & Borrowing
----

```rust
fn change(some_string: &String) {
    some_string.push_str("bar");
}
fn main() {
    let mut s = String::from("foo");
    change(s);
}
```

Compile Error
----
```bash
error[E0596]: cannot borrow `*some_string` as mutable, as it is behind a `&` reference
 --> src/main.rs:2:5
  |
1 | fn change(some_string: &String) {
  |                        ------- help: consider changing this to be a mutable reference: `&mut std::string::String`
2 |     some_string.push_str(", world");
  |     ^^^^^^^^^^^ `some_string` is a `&` reference, so the data it refers to cannot be borrowed as mutable
```

Fix: mutable ref
----

```rust
fn change(some_string: &mut String) {
    some_string.push_str("bar");
}
```
Problem:
```rust
let mut s = String::from("foo");
let r1 = &mut s;
let r2 = &mut s;
println!("{}, {}", r1, r2);
```

Complie Error
----
```bash
error[E0499]: cannot borrow `s` as mutable more than once at a time
 --> src/main.rs:4:14
  |
3 |     let r1 = &mut s;
  |              ------ first mutable borrow occurs here
4 |     let r2 = &mut s;
  |              ^^^^^^ second mutable borrow occurs here
5 |     println!("{} {}", r1, r2);
  |                       -- first borrow later used here
```

Lifetimes & Aliasing
----
* Two kinds of references: `&` and `&mut` with rules
    * A reference cannot outlive its referent
    * A mutable reference cannot be aliased ("variables and pointers _alias_ if the refer to overlapping regions of memory")
* So how do we use values belonging to other variables? Borrowing!

