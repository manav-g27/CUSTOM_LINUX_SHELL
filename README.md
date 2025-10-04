# Custom Linux Shell in C++ (POSIX-like)

A lightweight POSIX-style shell implemented in modern C++.  
This project is designed for learning OS concepts and to showcase systems programming skills.

## Features
- Command parsing and tokenization
- Pipes (`|`) across multiple commands
- Input/Output redirection (`<`, `>`)
- Background jobs using `&`
- Basic job control (`jobs`)
- Built-in commands: `cd`, `pwd`, `history`, `exit`
- Simple command history recording

## Build
```bash
custom-linux-shell && make
```

## Run
```bash
./shell
```

## Example
```
myshell> pwd
/home/user/custom-linux-shell
myshell> ls | grep cpp > out.txt
myshell> sleep 5 &
[Running in background]
myshell> jobs
[1] PID: 12345 - Running - sleep 5 &
myshell> history
1  pwd
2  ls | grep cpp > out.txt
3  sleep 5 &
4  jobs
5  history
```
