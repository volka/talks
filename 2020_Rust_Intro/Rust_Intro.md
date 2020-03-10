
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

Hello X!
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
* Writing Linux Kernel Modules, Operating Systems (->[Redox](https://www.redox-os.org))
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
    

Variables
----
```rust
let x = 42; // immutable integer
let mut y = 1; // mutable integer
```

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
Error without lifetime speficier on &str
```bash
error[E0106]: missing lifetime specifier
 --> src/main.rs:4:11
  |
4 |     name: &str,
  |           ^ expected lifetime parameter
```

Data Types - Enums
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

Control Flow - branch and loop
----
```rust
if x % 2 == 0 { println!("x is even!"); } 
else { println!("x is odd!"); }

// infinite loop without break
loop { println!("I'm alive!"); break; } 

let mut i = 0;
while i < 10 { println!("hello"); i = i + 1; }

let mut x = vec!["C++", "Rust", "Go", "Python"];
for text in x {
    println!("I like {}.", text);
}
for (i, text) in v.iter().enumerate() { /* ... */ }

while let Some(y) = x.pop() {
    println!("x = {}", y);
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

Constructors / Destructors
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
* Destructor: `std::ops::Drop::drop`
```rust
impl Drop for X {
    fn drop(&mut self) {}
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
* Impl only allowed in either trait module or type module!
* No inheritance!

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

Area covered by Lifetimes
----
```rust

let mut data = vec![1, 2, 3];
let x = &data[0];
println!("{}", x);
// This is OK, x is no longer needed
data.push(4);
```
```rust

let mut data = vec![1, 2, 3];
let x = &data[0];
data.push(4);
// This fails
println!("{}", x);
```
```rust
// ... also ok
if (condition()) {
    println!("{}", x);
    data.push(4);
} else {
    data.push(5);
}
```

Lifetimes & Aliasing
----
* Two kinds of references: `&` and `&mut` with rules
    * A reference cannot outlive its referent
    * A mutable reference cannot be aliased ("variables and pointers _alias_ if the refer to overlapping regions of memory")
    * Some intelligence in borrow checker, e.g. distinct fields in struct (but not slices/arrays)
* So how is this tracked?
    * Basically markers defining regions where things are "alive"
    * Local lifetimes mostly implicit (syntactic sugar / defaults)
    * Sane defaults keep code readable
* Lifetime annotations for explicitly "helping" the borrow checker

Local Lifetimes
----
```rust
let x = 1;
let y = &x;
let z = &y;
```
"desugared" (some invalid syntax)
```rust
'a: {
    let x: i32 = 42;
    'b: { // b "good enough"
        let y: &'b i32 = &'b x;
        'c: { // c "good enough"
            let z: &'c &'b i32 = &'c y;
        }

    }
}
```

Local Lifetimes - add outer lifetime
----
```rust
let x = 1;
let y = &x;
let z = &y;
z = y;
```
"desugared" (some invalid syntax)
```rust
'a: {
    let x: i32 = 42;
    'b: {
        let z: &'b i32;
        'c: {
            // must use 'b here because reference is passed to that scope
            let y: &'b i32 = &'b x;
            z = y;
        }

    }
}
```
TODO: http://arthurtw.github.io/2014/11/30/rust-borrow-lifetimes.html

Reference outliving referent
----
```rust
fn as_str(data: &u32) -> &str {
    let s = format!("{}", data);
    &s
}
// desugars to
fn as_str<'a>(data: &'a u32) -> &'a str {
    'b: {
        let s = format!("{}", data);
        return &'a s;
    }
}
```
* ref to s must outlive 'a, but defined in 'b, which is inside 'a
--> ERROR

Lifetime Elision
----
* Lifetime positions: input (fn arguments), output (fn result types)
* Elision rules
    * Each elided lifetime in input position -> distinct lifetime parameter
    * If exactly one input lifetime position, assign that to _all_ elided output lifetimes
    * If multiple input lifetime positions, but one is `&self` or `&mut self`, lifetime of `self` assgned to _all_ elided output lifetimes
    * Otherwise -> error to elide output lifetime

[Nomicon Lifetime Ellision](https://doc.rust-lang.org/nomicon/lifetime-elision.html)

Lifetime Elision Examples (from Nomicon)
----
```rust
fn print(s: &str);                                      // elided
fn print<'a>(s: &'a str);                               // expanded

fn debug(lvl: usize, s: &str);                          // elided
fn debug<'a>(lvl: usize, s: &'a str);                   // expanded

fn substr(s: &str, until: usize) -> &str;               // elided
fn substr<'a>(s: &'a str, until: usize) -> &'a str;     // expanded

fn get_str() -> &str;                                   // ILLEGAL

fn frob(s: &str, t: &str) -> &str;                      // ILLEGAL

fn get_mut(&mut self) -> &mut T;                        // elided
fn get_mut<'a>(&'a mut self) -> &'a mut T;              // expanded

fn args<T: ToCStr>(&mut self, args: &[T]) -> &mut Command                  // elided
fn args<'a, 'b, T: ToCStr>(&'a mut self, args: &'b [T]) -> &'a mut Command // expanded

fn new(buf: &mut [u8]) -> BufWriter;                    // elided
fn new<'a>(buf: &'a mut [u8]) -> BufWriter<'a>          // expanded
```

Lifetimes and Slices / Arrays
----
* Lifetimes are *dumb*, just markers
* Checker understands disjount struct fields, but not slices / arrays
* Works most of the time, sometimes needs help
```rust
let mut x = [1,2,3];
let a = &mut x[0];
let b = &mut x[1];
```
```bash
error[E0499]: cannot borrow `x[..]` as mutable more than once at a time
 --> src/lib.rs:4:18
  |
3 |     let a = &mut x[0];
  |                  ---- first mutable borrow occurs here
4 |     let b = &mut x[1];
  |                  ^^^^ second mutable borrow occurs here
5 |     println!("{} {}", a, b);
6 | }
  | - first borrow ends here
```

Lifetimes and Slices / Arrays
----
* May require falling back to unsafe code
```rust
// exposed by slices, consumes slice, returns two mutable slices
fn split_at_mut(&mut self, mid: usize) -> (&mut [T], &mut [T])`
{ /* some unsafe code in here */ }
```
* Iterators actually don't need unsafe code
    * Reference only one &mut ref at a time!

* Basically: fail on the "safe side", don't allow errors, but disallow some valid code!

Not covered but interesting ...
----
* [std](https://doc.rust-lang.org/std/) library
* [Modules and Packages](https://doc.rust-lang.org/book/ch07-00-managing-growing-projects-with-packages-crates-and-modules.html)
* Pointers / Wrappers - `Box<T>`, `Rc<T>`, `Arc<T>`, `Cell<T>`, `RefCell<T>`
* Standardized Serialization with [Serde](https://serde.rs/)
* Standardized SQL / ORM with [Diesel](http://diesel.rs/)
* Async / Await - see [Async Book](https://rust-lang.github.io/async-book/)
    * [Tokio](https://github.com/tokio-rs/tokio) / [Rayon](https://github.com/rayon-rs/rayon) async engines, [Rocket](https://rocket.rs/) or [Actix](https://github.com/actix/actix-web) web frameworks
* Macros
* FFI
* Details on Cargo


Thanks for the attention!
----

* Microsoft Slide taken from [Microsoft MSRC Security Research Github](https://github.com/microsoft/MSRC-Security-Research/blob/master/presentations/2019_02_BlueHatIL/2019_01%20-%20BlueHatIL%20-%20Trends%2C%20challenge%2C%20and%20shifts%20in%20software%20vulnerability%20mitigation.pdf)
* Also see [MSRC Blog](https://msrc-blog.microsoft.com/2019/07/16/a-proactive-approach-to-more-secure-code/)
* All Rust resources from [rust-lang.org](https://www.rust-lang.org)
